#include "inputprompt.h"
#include "ui_inputprompt.h"
#include <QFileDialog>
#include <iostream>

InputPrompt::InputPrompt(imageSelector *parent, QCommandLineParser& parser) :
    QDialog(parent),
    ui(new Ui::InputPrompt), loop(false), recurse(false) {
    ui->setupUi(this);
    this->parent = parent;

    //parser.value("i").toLatin1().data(), parser.isSet("r");
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
    inputFilepath = QFileDialog::getExistingDirectory(parent, QCoreApplication::translate("main", "Open directory"), "./", QFileDialog::ShowDirsOnly);
    ui->inputPath->setPlainText(inputFilepath);
    std::cout << "y u exit" << std::endl;
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
    parent->setup(inputFilepath, outputFilepath, loop, recurse);
    parent->show();
}

void InputPrompt::on_buttonBox_rejected() {
    std::cout << "Rejected settings" << std::endl;
}
