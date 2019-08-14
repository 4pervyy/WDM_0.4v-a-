#include "serverc.h"

ServerC::ServerC()
{

}

void ServerC::startServer(qint16 port)
{
    if (this->listen(QHostAddress::Any, port))
    {
        qDebug()<<"Listening";
    }
    else
    {
        qDebug()<<"Not listening";
    }
}

void ServerC::incomingConnection(int socketDescriptor) // подключение
{
    ssocket = new QTcpSocket(this);
    ssocket->setSocketDescriptor(socketDescriptor);
        connect(ssocket,SIGNAL(readyRead()),this,SLOT(sockReady()));
        connect(ssocket,SIGNAL(disconnected()),this,SLOT(sockDisc()));
    qDebug()<<socketDescriptor<< " Client connected";
    ssocket->write("Connect");
    qDebug()<<"Send client connect status - YES";
}

void ServerC::sockReady() //прием
{
    Data = ssocket->readAll();
    if(Data== "Connect_c"){
        QMessageBox::information(nullptr,"Информация","Conncet (S)");
    }
    else if (Data == "Wait"){
        if(ffsname.isEmpty()){
               QMessageBox::information(nullptr,"Ошибка","Файл не выбран");
        }
        else {
             ffs.setFileName(ffsname);
             QByteArray size = QByteArray::number(ffs.size());
             ssocket->write(size);
             ssocket->waitForBytesWritten();}
    }
    else if (Data =="Ready") {
        if (ffs.open(QIODevice::ReadOnly | QIODevice::Text)){
             QByteArray fromFile = ffs.readAll();
             if (fromFile.size() >= 1024) {
                 int step = 0;
                 int sizefile = fromFile.size()/1024;
                 int sizeBalance = fromFile.size() - (1024*sizefile);
                 QByteArray sendFile;
                 for (int i=0;i<sizefile;i++) {
                     for (int j=0;j<1024;j++) {
                        sendFile.append(fromFile[i+step]);
                     }
                   step+=1024;
                   ssocket->write(sendFile);
                   ssocket->waitForBytesWritten();
                }
                 if(sizeBalance > 0){
                     for (int k=0;k<sizeBalance;k++) {
                         sendFile.append(fromFile[k+(sendFile.size()-sizeBalance)]);
                     }
                     ssocket->write(sendFile);
                     ssocket->waitForBytesWritten();
                 }
             }
             else {
             ssocket->write(fromFile);
             ssocket->waitForBytesWritten();
        }}
        QMessageBox::information(nullptr,"Информация","Отправка файла");
        ssocket->write("Finish");
     ffs.close();
    }
     else {
         QMessageBox::information(nullptr,"Информация","No Connect(s)");
        }
}
void ServerC::sockDisc()
{
    qDebug()<<"Disconnect";
    ssocket->deleteLater();
}

ServerC::~ServerC(){}
void ServerC::stopServer(){
    qDebug()<<"Test";
   ssocket->flush();
}

