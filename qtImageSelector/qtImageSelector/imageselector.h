#ifndef IMAGESELECTOR_H
#define IMAGESELECTOR_H

#include <QMainWindow>
#include <QPainter>
#include "file_utilities.h"
#include <memory>

namespace Ui {
class imageSelector;
}

class imageSelector : public QMainWindow
{
    Q_OBJECT

public:
    explicit imageSelector(QWidget *parent = nullptr);
    void setup(QString& inputPath, QString& outputPath, bool loop, bool recurse);
    ~imageSelector() override;

private:
    QPixmap bkgnd;
    QPixmap bkgnd_scaled;
    std::unique_ptr<fileCopier> fs_handle;
    Ui::imageSelector *ui;
    std::vector<std::string> filepaths;
    std::vector<std::string> filenames;
    std::vector<bool> selected;
    size_t i = 0;
    bool loop;
    void rotate(int degrees);

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void paintEvent(QPaintEvent *) override;
};

#endif // IMAGESELECTOR_H
