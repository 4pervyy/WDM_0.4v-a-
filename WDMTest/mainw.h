#ifndef MAINW_H
#define MAINW_H

#include <QThread>
#include <QMainWindow>
#include <QtNetwork>
#include <QTcpServer>
#include <QTcpSocket>
#include <QRegExpValidator>
#include <QMessageBox>

#include "winsock2.h"
#include "iphlpapi.h"
#include "icmpapi.h"


#include <QtCharts>
#include <QString>

//Additional lib

#include "aboutw.h"
#include "settingw.h"
#include "videop.h"
#include "client.h"
#include "serverw.h"

#define NET_SERVER_2000
//#define ALPHA

#include<sniffer.h>


namespace Ui {
class MainW;
class ThreadPing;
}

class MainW : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainW(QWidget *parent = nullptr);
    ~MainW();
    class sender senderd;
    QTcpSocket* socket;
    QByteArray Data;
    friend class ThreadPing;
    Ui::MainW *ui;
    VideoP *vp;
    void vMessage(QString,QString);
    void on_Packet_received();
    Controller *controllerthread;

private slots:
    void on_Button_ping_clicked();
    void on_Button_Server_clicked();
    void on_Button_Client_clicked();
    void aboutv();
    void lab_wp();
    void settingv();
    void closedw();

    void on_Button_startsniffer_clicked();
    void on_button_stopsniffer_clicked();
    void on_sniffer_thread_message(const QString &s);
    void on_sniffer_thread_new_upload_size(const QString &s);
    void on_sniffer_thread_new_download_size(const QString &s);
    void on_sniffer_thread_new_upload_speed(const QString &s);
    void on_sniffer_thread_new_download_speed(const QString &s);


private:
    ServerW *aw;
    AboutW *abw;
    SettingW *setw;
    Client *clnt;
    ServerC *wsr;

};


class ThreadPing : public QThread {
private:
  public:
    class sender senderd;
    void run();
};


#endif // MAINW_H
