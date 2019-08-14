#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QTime>
#include <QTimer>

//
#include "settingw.h"

namespace Ui {
class Client;
}

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    ~Client();
    QTimer *tmr;
    QTcpSocket* socket;
    QByteArray Data;
    bool Transfer_ready = false;
    class sender _sender;
    int timers=0;

public slots:
    void sockReady();
    void sockDisc();

private slots:
    void on_Start_button_clicked();
    void on_CAccept_button_clicked();
    void timesend();

private:
    Ui::Client *ui;
};


#endif // CLIENT_H
