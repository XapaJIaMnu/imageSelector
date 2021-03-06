#include "imageselector.h"
#include "ui_imageselector.h"
#include <QScreen>
#include <QPainter>
#include <QKeyEvent>
#include <QMessageBox>
#include <QTransform>
#include "inputprompt.h"

imageSelector::imageSelector(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::imageSelector) {
    ui->setupUi(this);
}

void imageSelector::setup(QString& inputPath, QString& outputPath, bool loop, bool recurse) {
    this->loop = loop;

    std::string fsError("");
    fileUtilities::fileWalker(inputPath.toUtf8().data(), this->filepaths, this->filenames, recurse, fsError);
    if (fsError != "") {
        QMessageBox error = QMessageBox(this);
        error.setText(QString("Warning! Not all files in:\n") + inputPath + QString( "\nhave been processed, due to encoutering an error:\n") + QString(fsError.c_str()));
        error.setWindowTitle(QString("Warning"));
        error.setStandardButtons(QMessageBox::Ok);
        error.exec();
    }
    this->fs_handle = std::make_unique<fileCopier>(fileCopier(outputPath.toUtf8().data()));

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

    //Calculate help text dimensions
    font = QFont("Noto", 16, QFont::Normal);
    QFontMetrics fm(font);
    helpWidth = fm.width(helptxt);
    helpHeight = fm.height()*3+10; //three new lines

    //QTimer::singleShot(20000, this, [this](){this->showHelp = false; this->update();});
}

void imageSelector::rotate(int degrees) {
    QTransform transform;
    QTransform trans = transform.rotate(degrees);
    bkgnd = bkgnd.transformed(trans);
}

void imageSelector::drawHelpText(QPainter& p, QRect& rect) {
    //Draw a semi-transparent overlay
    p.setBrush(QBrush(QColor(24, 37, 51, 220)));
    QPoint centerBottom = rect.bottomLeft();
    centerBottom.setX(rect.bottomRight().x()/2);
    p.drawEllipse(centerBottom, helpWidth, helpHeight);

    //Write into it
    p.setPen(QPen(Qt::white));
    p.setFont(font);
    p.drawText(rect, Qt::AlignBottom, helptxt);
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

    //Draw help text
    if (showHelp) {
        drawHelpText(painter, devRect);
    }

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
    } else if (e->key() == Qt::Key_W) {
        this->rotate(-90);
        this->update();
    } else if (e->key() == Qt::Key_E) {
        this->rotate(90);
        this->update();
    } else if (e->key() == Qt::Key_H) {
        this->showHelp = !this->showHelp;
        this->update();
    }
}

void imageSelector::mousePressEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton) {
        this->showHelp = !this->showHelp;
        this->update();
    }
}


imageSelector::~imageSelector() {
    delete ui;
}
