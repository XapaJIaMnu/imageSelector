#include "imageselector.h"
#include "ui_imageselector.h"
#include <QScreen>
#include <QPainter>
#include <QKeyEvent>
#include <iostream>
#include "inputprompt.h"

imageSelector::imageSelector(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::imageSelector) {
    ui->setupUi(this);
}

void imageSelector::setup(QString inputPath, QString outputPath, bool loop, bool recurse) {
    this->loop = loop;
    //@TODO LEAKING
    fileIterator* files = new fileIterator (inputPath.toLatin1().data(), recurse);
    fileCopier* copier= new fileCopier(outputPath.toLatin1().data());
    this->fs_handle = copier;
    this->filenames = files->filenames;
    this->filepaths = files->paths;

    //Set the "selected" vector
    selected.resize(this->filenames.size());

    //Set background window size ot .7 of the main screen
    QRect mainScreenSize = QGuiApplication::primaryScreen()->availableGeometry();
    int width = static_cast<int>(mainScreenSize.width()*.7);
    int height = static_cast<int>(mainScreenSize.height()*.7);
    this->resize(width, height);

    bkgnd = QPixmap(filepaths[i].c_str());
    QPalette palette;
    palette.setBrush(QPalette::Window, Qt::black);
    this->setPalette(palette);
}


void imageSelector::paintEvent(QPaintEvent *) {

    //Set Window title
    this->setWindowTitle(QCoreApplication::instance()->applicationName() + " - " + filenames[i].c_str());

    QPainter painter(this);
    bkgnd_scaled = bkgnd.scaled(this->size(), Qt::KeepAspectRatio);
    QRect rect = bkgnd_scaled.rect();
    QRect devRect(0, 0, painter.device()->width(), painter.device()->height());
    rect.moveCenter(devRect.center());
    painter.drawPixmap(rect.topLeft(), bkgnd_scaled);

    //Draw a green dot in the top left corner to show that an image has been selected
    if (selected[i]) {
        painter.setBrush(QBrush(Qt::green));
        painter.drawEllipse(10,10, 50, 50);
    }
}

void imageSelector::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Left) {
        if (i > 0) {
            i--;
            bkgnd = QPixmap(filepaths[i].c_str());
            this->update();
        } else if (loop) {
            i = filepaths.size() - 1;
            bkgnd = QPixmap(filepaths[i].c_str());
            this->update();
        }
    } else if (e->key() == Qt::Key_Right) {
        if (i < filepaths.size() - 1) {
            i++;
            bkgnd = QPixmap(filepaths[i].c_str());
            this->update();
        } else if (loop) {
            i = 0;
            bkgnd = QPixmap(filepaths[i].c_str());
            this->update();
        }
    } else if (e->key() == Qt::Key_S) {
        fs_handle->copy(filepaths[i]);
        selected[i] = true;
        this->update();
    } else if (e->key() == Qt::Key_D) {
        fs_handle->uncopy(filepaths[i]);
        selected[i] = false;
        this->update();
    }
}


imageSelector::~imageSelector() {
    delete ui;
}
