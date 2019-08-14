#include "videop.h"
#include "ui_videop.h"

VideoP::VideoP(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VideoP)
{
    ui->setupUi(this);
    player = new QMediaPlayer(this);
    vw = new QVideoWidget(this);
    //this->setCentralWidget(vw);
    vw->setGeometry(0,0,300,200);
    ui->VideoP_w = new QVideoWidget;
    player->setVideoOutput(vw);
}

VideoP::~VideoP()
{
    delete ui;
}

void VideoP::on_Button_open_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,"Open a File","","Video File (*.*)");
    on_Button_stop_clicked();
    player->setMedia(QUrl::fromLocalFile(filename));
    on_Button_start_clicked();
}

void VideoP::on_Button_start_clicked()
{
     player->play();
}

void VideoP::on_Button_pause_clicked()
{
    player->pause();
}

void VideoP::on_Button_stop_clicked()
{
    player->stop();
}
