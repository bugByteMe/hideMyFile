#include "pageone.h"
#include "ui_pageone.h"
#include "tool.h"
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <QMessageBox>

PageOne::PageOne(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PageOne)
{
    ui->setupUi(this);
    QPalette palette;
    palette.setColor(QPalette::Background, Qt::white);
    this->setPalette(palette);
    ui->write->setDisabled(true);
}

PageOne::~PageOne()
{
    delete ui;
}

void PageOne::setStatus(){
    if(picCapacity*tunnel >= fileSize && savePathStr != "" && filePathStr != ""){
        ui->write->setDisabled(false);
        ui->butStatus->setText("已准备完成");
    }else if(savePathStr == "" || filePathStr == ""){
        ui->write->setDisabled(true);
        ui->butStatus->setText("未设置完成");
    }else{
        ui->write->setDisabled(true);
        ui->butStatus->setText("容量不足");
    }
}

void PageOne::on_selectFile_clicked()
{
    QString tmpstr = getFilePath(ui->filePath, this);
    if(tmpstr != ""){
       filePathStr = tmpstr;
       QFile file(filePathStr);
       fileSize = file.size();
    }
    setStatus();
}

void PageOne::on_savePath_clicked()
{
    QString tmpstr = getDir(ui->savePathLabel, this);
    if(tmpstr != "") savePathStr = tmpstr;
    setStatus();
}

void PageOne::paintEvent(QPaintEvent *event){
    ui->label->resize(ui->widget->size());
    ui->label->move(0,0);
    previewImg.load(QString::fromStdString(prePicName));
    QSize labelSize = ui->label->size();
    previewImg = previewImg.scaled(labelSize, Qt::IgnoreAspectRatio);
    ui->label->setPixmap(QPixmap::fromImage(previewImg));
}

void PageOne::on_write_clicked()
{
    binStr = readBinStr(filePathStr.toLocal8Bit().toStdString(), ui->progressBar, dataLen, ui->status);
    writeToPic(writePicName,filePathStr.toStdString(), savePathStr.toStdString(), binStr, dataLen, tunnel, ui->progressBar, ui->status);
    ui->progressBar->setValue(100);
    ui->status->setText("完成！");
    QMessageBox::information(this,"提示","完成！图片保存为output.png");
}

void PageOne::on_randPic_clicked()
{
    QFile file("previewImg.png");
    if(file.exists()){
        file.remove();
    }
    srand((unsigned)time(0));
    int a = rand()%8;
    writePicName = ":/pic/";
    writePicName += a+'0';
    writePicName += "_32.png";
    prePicName = ":/pic/";
    prePicName += a+'0';
    prePicName += "_s.png";
    picCapacity = 3107520;
    setStatus();
}

void PageOne::on_customPic_clicked()
{
    std::string tmpstr = costumPicPath(this);
    if(tmpstr != ""){
        writePicName = tmpstr;
        prePicName = "previewImg.png";
        QImage img(QString::fromStdString(writePicName));
        picCapacity = img.width()*(img.height()-2)*3/8;
        char ch[50];
        snprintf(ch,50,"通道数：%d/8 最大容量：%lldb",tunnel,(long long)tunnel*(long long)picCapacity);
        QString qstr = QString::fromStdString(ch);
        ui->sliderLabel->setText(qstr);
    }
    setStatus();
}

void PageOne::on_slider_sliderMoved(int position)
{
    char ch[50];
    snprintf(ch,50,"通道数：%d/8 最大容量：%db",position,position*picCapacity);
    QString qstr = QString::fromStdString(ch);
    ui->sliderLabel->setText(qstr);
    tunnel = position;
    setStatus();
}
