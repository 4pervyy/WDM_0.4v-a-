#ifndef SETTINGW_H
#define SETTINGW_H


#include "string.h"
#include "stdio.h"
#include <QRegExpValidator>
#include <QDialog>
#include <QtNetwork>
#include <QString>


namespace Ui {
class SettingW;
}

class SettingW : public QDialog
{
    Q_OBJECT

public:
    explicit SettingW(QWidget *parent = nullptr);
    ~SettingW();

private slots:
    void on_pushButton_clicked();

    void on_SandE_Button_clicked();

private:
    Ui::SettingW *ui;
};

class sender{
public:
    QString ip();
    int maxping();
    int numberpacket();
    int numberpacketp(int np){
        return np;
    }
    double timeping(double tp){
        return tp;
    }
    double sizepacket(double sp){
        return sp;
    }
    quint16 numberport();
};

//extern QString ip2;


#endif // SETTINGW_H
