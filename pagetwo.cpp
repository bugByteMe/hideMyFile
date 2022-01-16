#include "pagetwo.h"
#include "ui_pagetwo.h"
#include "tool.h"
#include <QMessageBox>

Pagetwo::Pagetwo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Pagetwo)
{
    ui->setupUi(this);
    QPalette palette;
    palette.setColor(QPalette::Background, Qt::white);
    this->setPalette(palette);
    ui->get->setDisabled(true);
}

Pagetwo::~Pagetwo()
{
    delete ui;
}

void Pagetwo::on_selectPic_clicked()
{
    picPath = getPicPath(ui->picPathLb, this).toStdString();
    if(picPath != "" && savePath != ""){
        ui->get->setDisabled(false);
        ui->butStatus->setText("已准备完成");
    }
}

void Pagetwo::on_selectPath_clicked()
{
    savePath = getDir(ui->savePathLb, this).toStdString();
    if(picPath != "" && savePath != ""){
        ui->get->setDisabled(false);
        ui->butStatus->setText("已准备完成");
    }
}

void Pagetwo::on_get_clicked()
{
    getFile(picPath, savePath, ui->progressBar, ui->status);
    QMessageBox::information(this,"提示","完成！");
}
