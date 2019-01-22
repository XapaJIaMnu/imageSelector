#include "imageselector.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QCommandLineParser>
#include <iostream>
#include "file_utilities.h"

void parseCommandLine(QCommandLineParser &parser) {
    parser.setApplicationDescription("A small program to save some images and discard others.");
    parser.addHelpOption();

    //An option with a value
    QCommandLineOption imageDir(QStringList() << "i" << "image-dir", "The location of the input image <directory>.", "directory");
    QCommandLineOption tgtDir(QStringList() << "t" << "target-dir", "The <tgtDirectory> where we will keep the images.", "tgtDirectory");
    parser.addOption(imageDir);
    parser.addOption(tgtDir);
    // Process the actual command line arguments given by the user
    parser.process(QApplication::arguments());

    QString imageSrc = parser.value(imageDir);
    QString imageTgt = parser.value(tgtDir);
    std::cout << imageSrc.toLatin1().data() << " " << imageTgt.toLatin1().data() << std::endl;
}

int main(int argc, char *argv[]) {
    const char * input = "/home/dheart/Pictures/Zurich";
    const char * output = "/tmp/dir";
    fileIterator files(input, true);
    fileCopier copier(output);
    QApplication a(argc, argv);
//    QCommandLineParser parser;
//    parseCommandLine(parser);
    QDesktopWidget widget;
    imageSelector w(files.paths, files.filenames, &copier);
    w.show();
//    for (size_t i = 0; i < files.filenames.size(); i++) {
//        std::cout << files.paths[i] << std::endl;
//        std::cout << files.filenames[i] << std::endl;
//    }

    return a.exec();
}
