#ifndef PAGEONE_H
#define PAGEONE_H

#include <QDialog>
#include <QString>
#include <string>

namespace Ui {
class PageOne;
}

class PageOne : public QDialog
{
    Q_OBJECT

public:
    explicit PageOne(QWidget *parent = 0);
    ~PageOne();

private slots:
    void on_selectFile_clicked();

    void on_savePath_clicked();

    void on_write_clicked();

    void on_randPic_clicked();

    void on_customPic_clicked();

    void on_slider_sliderMoved(int position);

private:
    Ui::PageOne *ui;
    int tunnel = 1;
    long long picCapacity = 3107520;
    long long fileSize = 0;
    QString filePathStr;
    QString savePathStr;
    QImage previewImg;
    std::string binStr;
    std::string prePicName = ":/pic/1_s.png", writePicName = ":/pic/1_32.png";
    unsigned long long dataLen;
    void paintEvent(QPaintEvent *event);
    void setStatus();
};

#endif // PAGEONE_H
