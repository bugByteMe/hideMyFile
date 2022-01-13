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
}

PageOne::~PageOne()
{
    delete ui;
}

void PageOne::on_selectFile_clicked()
{
    filePathStr = getFilePath(ui->filePath, this);
}

void PageOne::on_savePath_clicked()
{
    savePathStr = getDir(ui->savePathLabel, this);
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
    /*
    freopen("out.txt","w",stdout);
    std::cout<<binStr<<std::endl;
    fclose(stdout);
    */
    //std::string fileName, std::string savePath, std::string binStr, unsigned long long dataLen
    writeToPic(writePicName,filePathStr.toStdString(), savePathStr.toStdString(), binStr, dataLen, ui->progressBar, ui->status);
    ui->progressBar->setValue(100);
    ui->status->setText("完成！");
    QMessageBox::information(this,"提示","完成！图片保存为output.png");
}

void PageOne::on_randPic_clicked()
{
    srand((unsigned)time(0));
    int a = rand()%8;
    writePicName = ":/pic/";
    writePicName += a+'0';
    writePicName += "_32.png";
    prePicName = ":/pic/";
    prePicName += a+'0';
    prePicName += "_s.png";
}
