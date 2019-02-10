#include "inputprompt.h"
#include "ui_inputprompt.h"
#include <QFileDialog>
#include "file_utilities.h"
#include <QMessageBox>

InputPrompt::InputPrompt(QWidget *parent, QCommandLineParser& parser) :
    QDialog(parent),
    ui(new Ui::InputPrompt) {
    ui->setupUi(this);
    this->setParent(parent);
    this->setWindowTitle(QCoreApplication::instance()->applicationName());

    ui->inputPath->setPlainText(parser.value("i"));
    ui->outputPath->setPlainText(parser.value("t"));
    if (parser.isSet("r")) {
        recurse = true;
        ui->recurseButton->setCheckState(Qt::Checked);
    }
    if (parser.isSet("l")) {
        loop = true;
        ui->loopButton->setCheckState(Qt::Checked);
    }

}

InputPrompt::~InputPrompt() {
    delete ui;
}

void InputPrompt::on_imagesSelect_clicked() {
    inputFilepath = QFileDialog::getExistingDirectory(this, QCoreApplication::translate("main", "Open directory"), "./", QFileDialog::ShowDirsOnly);
    ui->inputPath->setPlainText(inputFilepath);
}

void InputPrompt::on_outputFolder_clicked() {
    outputFilepath = QFileDialog::getExistingDirectory(this, QCoreApplication::translate("main", "Open directory"), "./", QFileDialog::ShowDirsOnly);
    ui->outputPath->setPlainText(outputFilepath);
}


void InputPrompt::on_inputPath_textChanged() {
    inputFilepath = ui->inputPath->toPlainText();
}


void InputPrompt::on_outputPath_textChanged() {
    outputFilepath = ui->outputPath->toPlainText();
}

void InputPrompt::on_loopButton_stateChanged(int) {
    loop = ui->loopButton->isChecked();
}

void InputPrompt::on_recurseButton_stateChanged(int) {
    recurse = ui->recurseButton->isChecked();
}

void InputPrompt::on_buttonBox_accepted() {
    QMessageBox error = QMessageBox(this);
    std::string fserror("");
    if (!fileUtilities::checkIfExistOrDir(inputFilepath.toUtf8().data())) {

        error.setText(QString("Error! The input directory:\n") + inputFilepath + QString( "\ndoes not exist!"));
        error.setWindowTitle(QString("Invalid path error"));
        error.setStandardButtons(QMessageBox::Ok);
        error.exec();
        return;
    } else if (!fileUtilities::checkIfImagesExist(inputFilepath.toUtf8().data(), recurse, fserror)) {
        if (fserror == "") {
            error.setText(QString("Error! The input directory:\n") + inputFilepath + QString( "\ndoes not contain any image files!"));
            error.setWindowTitle(QString("Invalid path error"));
            error.setStandardButtons(QMessageBox::Ok);
            error.exec();
            return;
        } else {
            error.setText(QString("Error opening files in the input directory:\n") + inputFilepath + QString("\n") + QString(fserror.c_str()));
            error.setWindowTitle(QString("Invalid path error"));
            error.setStandardButtons(QMessageBox::Ok);
            error.exec();
            return;
        }
    }

    int writeable = fileUtilities::checkIfWritable(outputFilepath.toUtf8().data());
    if (writeable == 1) {
        error.setText(QString("Error! The output path:\n") + outputFilepath + QString( "\npoints to a file and not a directory!"));
        error.setWindowTitle(QString("Invalid path error"));
        error.setStandardButtons(QMessageBox::Ok);
        error.exec();
    } else if (writeable == 2) {
        error.setText(QString("Error! The output path:\n") + outputFilepath + QString( "\nis not writeable!"));
        error.setWindowTitle(QString("Invalid path error"));
        error.setStandardButtons(QMessageBox::Ok);
        error.exec();
    } else if (writeable == 0){
        this->accept();
    }
}

void InputPrompt::on_pushButton_clicked() {
    QMessageBox help = QMessageBox(this);
    QString helpoutput("<p><b>imageSelector</b> is a simple program that lets you browse through your photo collection and easily select images to share from it.</p>\
<p><i>Input Folder</i> is your image collection directory.</p>\
<p><i>Output Folder</i> is where the selected photos will be saved.</p>\
<p>Once all paths and options are chosen, the image viewer window opens. This is how to use it:<br><br>\
<b>LeftArrow</b> and <b>RightArrow</b> go to the previous and next image respectively.<br><br>\
<b>s</b> saves the image to the  output directory. A green dot will appear in the top left corner to mark the image as selected.<br><br>\
<b>d</b> removes the image from the output directory.<br><br>\
<b>w</b> rotates the image left.<br><br>\
<b>e</b> rotates the image right.</p>");
    help.setText(helpoutput);
    help.setWindowTitle(QString("Help"));
    help.setStandardButtons(QMessageBox::Ok);
    help.exec();
}
