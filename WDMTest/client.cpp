#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
        tmr = new QTimer(this);
    connect(tmr,&QTimer::timeout,this,&Client::timesend);
        connect(socket,SIGNAL(readyRead()),this,SLOT(sockReady()));
        connect(socket,SIGNAL(disconnected()),this,SLOT(sockDisc()));

}

Client::~Client()
{
    delete ui;
}

void Client::on_CAccept_button_clicked()
{
    if (socket->isOpen()){
         socket->write("Wait");
         socket->waitForBytesWritten(500);
         tmr->start(1000);
    }
    else {
        QMessageBox::information(nullptr,"Статус","Не подключенно");
    }
}

void Client::on_Start_button_clicked()
{
    if(_sender.ip().isEmpty() ){
        QMessageBox::information(nullptr,"Ошибка", "Не указан IP");
    }
    else{
    ui->sclient->append("Start client");
    socket->connectToHost(_sender.ip(),5555);}
}

void Client::sockDisc()
{
socket->deleteLater();
}

void Client::sockReady()
{
if (socket->waitForConnected(500))
{
    Data = socket->readAll();
       if(Data == "Connect"){
           QMessageBox::information(nullptr,"Информация","Connect (c)");
           socket->write("Connect_c");
       }
       else if (Data == "Finish"){
            tmr->stop();
            ui->sclient->append("Time: " + timers);
            ui->sclient->append("File accept");
            QMessageBox::information(nullptr,"Информация","Передача завершена");
            Transfer_ready = false;
            timers=0;

       }
       else {
           if(Transfer_ready == true){//recive::file
            QFile acceptfile;
            acceptfile.setFileName("AcceptFile");
            acceptfile.open(QIODevice::WriteOnly);
            acceptfile.write(Data);
            acceptfile.close();
           }
           else if (Transfer_ready == false) {//size::file
                ui->sclient->append("File size: " + Data + " Byte");
                socket->write("Ready");
                Transfer_ready = true;
           }
           else {
               ui->sclient->append("Error  transfer");
           }
       }

   }
}
void Client::timesend(){
   timers++;
}
