#include "imageselector.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QCommandLineParser>
#include <iostream>
#include "file_utilities.h"

void parseCommandLine(QCommandLineParser &parser) {
    parser.setApplicationDescription("A small program to identify images to keep from a collection and discard others.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOptions({
            {{"r", "recursive"},
                QCoreApplication::translate("main", "Load images from multiple subdirectories recursively")},
            {{"l", "loop"},
                QCoreApplication::translate("main", "After reaching the final image, advance back to the first one")},
            {{"t", "target-directory"},
                QCoreApplication::translate("main", "The <tgtDirectory> where we will keep the images."),
                QCoreApplication::translate("main", "tgtDirectory"),
                          QString("/tmp/dir")}, //default value for testing
            {{"i", "images-directory"},
                QCoreApplication::translate("main", "The location of the input image <directory>."),
                QCoreApplication::translate("main", "directory"),
                          QString("/home/dheart/Pictures/Zurich")}, //default value for testing
        });
    parser.process(QApplication::arguments());
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setApplicationVersion(QString("0.1"));
    a.setApplicationName(QString("imageSelector"));
    QCommandLineParser parser;
    parseCommandLine(parser);

    fileIterator files(parser.value("i").toLatin1().data(), parser.isSet("r"));
    fileCopier copier(parser.value("t").toLatin1().data());
    imageSelector w(files.paths, files.filenames, parser.isSet("l"), &copier);
    w.show();

    return a.exec();
}
