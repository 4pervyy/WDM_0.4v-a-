#ifndef SERVERC_H
#define SERVERC_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QObject>
#include <QThread>
#include <QByteArray>

//
#include "settingw.h"

class ServerC: public QTcpServer
{
    Q_OBJECT
public:
    ServerC();
    ~ServerC();
    QTcpSocket* ssocket;
    QByteArray Data;
    QFile ffs;
    QString  ffsname;

public slots:
    void startServer(qint16 port);
    void incomingConnection(qintptr socketDescriptor);
    void sockReady();
    void sockDisc();
    void stopServer();
};

#endif // SERVERC_H
