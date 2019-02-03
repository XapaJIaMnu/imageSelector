#ifndef INPUTPROMPT_H
#define INPUTPROMPT_H

#include <QDialog>
#include <QCommandLineParser>
#include "imageselector.h"

namespace Ui {
class InputPrompt;
}

class InputPrompt : public QDialog
{
    Q_OBJECT

public:
    explicit InputPrompt(QWidget *parent, QCommandLineParser& parser);
    ~InputPrompt();

private slots:
    void on_imagesSelect_clicked();

    void on_outputFolder_clicked();

    void on_inputPath_textChanged();

    void on_outputPath_textChanged();

    void on_loopButton_stateChanged(int arg1);

    void on_recurseButton_stateChanged(int arg1);

    void on_buttonBox_accepted();

private:
    Ui::InputPrompt *ui;
public:
    QString inputFilepath;
    QString outputFilepath;
    bool loop = false;
    bool recurse = false;
};

#endif // INPUTPROMPT_H
