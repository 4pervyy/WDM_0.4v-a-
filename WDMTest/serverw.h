#ifndef SERVERW_H
#define SERVERW_H


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

#include "settingw.h"
#include "serverc.h"


namespace Ui {
class ServerW;
}

class ServerW : public QDialog
{
    Q_OBJECT

public:
    explicit ServerW(QWidget *parent = nullptr);
    ~ServerW();
     ServerC SVC;
     class sender senderd;

private slots:
     void on_CreateFileS_clicked();
     void on_OpenFileS_clicked();
     void on_StartS_clicked();

private:
     Ui::ServerW *ui;
};

class ThreadCreateFile : public QThread{
public:
    QFile save;
    QString filename;
    int filesize;
    void run(){
        save.setFileName(filename);
        save.open(QIODevice::Append);
        if(filesize ==0){
            QMessageBox::information(nullptr,"Ошибка","Неверный размер");
        }
        else if (filename.isEmpty() ==true) {
            QMessageBox::information(nullptr,"Ошибка","Нет имени");
        }
        else {
            for (int i = 0;i<filesize;i++) {
            save.write("a");
            }
        }
        save.close();
    }
};
#endif // SERVERW_H
