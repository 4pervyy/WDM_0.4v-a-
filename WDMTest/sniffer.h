#ifndef SNIFFER_H
#define SNIFFER_H

#include <QMainWindow>
#include <QtNetwork>
#include <QRegExpValidator>
#include "winsock2.h"
#include "iphlpapi.h"
#include "icmpapi.h"
#include <QtCharts>

class Worker : public QObject
{
    Q_OBJECT

public slots:
    void doWork(const QString &parameter);

public:
    bool abort;

signals:
    void resultReady(const QString &result);
    void newUploadSize(const QString &result);
    void newDownloadSize(const QString &result);
    void newUploadSpeed(const QString &result);
    void newDownloadSpeed(const QString &result);
};



class Controller : public QObject
{
    Q_OBJECT
    QThread workerThread;
public:

    Worker *worker;

    Controller() {
        QGuiApplication::topLevelWindows();
        worker = new Worker;
        worker->moveToThread(&workerThread);
        connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(this, &Controller::operate, worker, &Worker::doWork);
        connect(worker, &Worker::resultReady, this, &Controller::handleResults);
        connect(worker, &Worker::newUploadSize, this, &Controller::changeUploadSize);
        connect(worker, &Worker::newDownloadSize, this, &Controller::changeDownloadSize);
        connect(worker, &Worker::newUploadSpeed, this, &Controller::changeUploadSpeed);
        connect(worker, &Worker::newDownloadSpeed, this, &Controller::changeDownloadSpeed);
        workerThread.start();
    }
    ~Controller() {
        workerThread.quit();
        workerThread.wait();
    }



public slots:

private:

signals:
    void handleResults(const QString &result);
    void changeUploadSize(const QString &result);
    void changeDownloadSize(const QString &result);
    void changeUploadSpeed(const QString &result);
    void changeDownloadSpeed(const QString &result);
    void operate(const QString &);

};



#endif // SNIFFER_H

