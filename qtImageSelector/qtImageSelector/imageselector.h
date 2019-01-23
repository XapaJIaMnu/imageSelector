#ifndef IMAGESELECTOR_H
#define IMAGESELECTOR_H

#include <QMainWindow>
#include <QPainter>
#include "file_utilities.h"

namespace Ui {
class imageSelector;
}

class imageSelector : public QMainWindow
{
    Q_OBJECT

public:
    explicit imageSelector(std::vector<std::string>&, std::vector<std::string>&, bool, fileCopier *, QWidget *parent = nullptr);
    ~imageSelector() override;

private:
    QPixmap bkgnd;
    QPixmap bkgnd_scaled;
    fileCopier * fs_handle;
    Ui::imageSelector *ui;
    std::vector<std::string>& filepaths;
    std::vector<std::string>& filenames;
    size_t i;
    bool loop;

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void paintEvent(QPaintEvent *) override;
};

#endif // IMAGESELECTOR_H
