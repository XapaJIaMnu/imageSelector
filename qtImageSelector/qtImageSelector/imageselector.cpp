#include "imageselector.h"
#include "ui_imageselector.h"
#include <QScreen>
#include <QPainter>
#include <QKeyEvent>
#include <iostream>

imageSelector::imageSelector(std::vector<std::string>& files, std::vector<std::string>& names,
                             bool looping, fileCopier * handle, QWidget *parent) :
    QMainWindow(parent), fs_handle(handle), ui(new Ui::imageSelector),
    filepaths(files), filenames(names), i(0), loop(looping)  {
    ui->setupUi(this);

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
    } else if (e->key() == Qt::Key_D) {
        fs_handle->uncopy(filepaths[i]);
    }
}


imageSelector::~imageSelector() {
    delete ui;
}
