#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pageone.h"
#include "pagetwo.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_butOne_clicked();

    void on_butTwo_clicked();

private:
    Ui::MainWindow *ui;
    PageOne *ptrOne;
    Pagetwo *ptrTwo;
};

#endif // MAINWINDOW_H
