#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QDateTime>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap(":/pic/startPic.png");
    QSplashScreen screen(pixmap);
    screen.show();
    #if 1
        QDateTime n=QDateTime::currentDateTime();
        QDateTime now;
        do{
            now=QDateTime::currentDateTime();
            a.processEvents();
        } while (n.secsTo(now)<=1);//5为需要延时的秒数
    #endif
    MainWindow w;
    w.show();
    screen.finish(&w);
    return a.exec();
}
