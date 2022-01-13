#ifndef PAGETWO_H
#define PAGETWO_H

#include <QDialog>
#include <QString>
#include <string>

namespace Ui {
class Pagetwo;
}

class Pagetwo : public QDialog
{
    Q_OBJECT

public:
    explicit Pagetwo(QWidget *parent = 0);
    ~Pagetwo();

private slots:
    void on_selectPic_clicked();

    void on_selectPath_clicked();

    void on_get_clicked();

private:
    Ui::Pagetwo *ui;
    std::string picPath;
    std::string savePath;
};

#endif // PAGETWO_H
