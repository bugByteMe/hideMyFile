#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pageone.h"
#include "pagetwo.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPalette palette;
    palette.setColor(QPalette::Background, Qt::white);
    this->setPalette(palette);
    ptrOne = new PageOne();
    ptrTwo = new Pagetwo();
    ui->gridLayout->addWidget(ptrOne);
    ui->gridLayout->addWidget(ptrTwo);
    ptrTwo->close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_butOne_clicked()
{
    ptrOne->close();
    ptrTwo->close();
    ptrOne->show();
}

void MainWindow::on_butTwo_clicked()
{
    ptrTwo->close();
    ptrOne->close();
    ptrTwo->show();
}
