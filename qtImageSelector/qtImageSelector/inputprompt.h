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
    explicit InputPrompt(imageSelector *parent, QCommandLineParser& parser);
    ~InputPrompt();

private slots:
    void on_imagesSelect_clicked();

    void on_outputFolder_clicked();

    void on_inputPath_textChanged();

    void on_outputPath_textChanged();

    void on_loopButton_stateChanged(int arg1);

    void on_recurseButton_stateChanged(int arg1);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::InputPrompt *ui;
    QString inputFilepath;
    QString outputFilepath;
    bool loop;
    bool recurse;
    imageSelector * parent;
};

#endif // INPUTPROMPT_H
