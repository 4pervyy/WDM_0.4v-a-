#include "serverw.h"
#include "ui_serverw.h"


ServerW::ServerW(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerW)
{
    ui->setupUi(this);
    ui->Readyfs->hide();
}

ServerW::~ServerW()
{
    delete ui;
}


//button
void ServerW::on_CreateFileS_clicked()
{
    ThreadCreateFile tcf;
    tcf.filename = ui->LE_NameFile->text();
    tcf.filesize = ui->SB_FileSize->value();
    tcf.run();
}

void ServerW::on_OpenFileS_clicked()
{
     SVC.ffsname = QFileDialog::getOpenFileName(nullptr,"Open a File","","File (*.*)");
     ui->Infofs->hide();
     ui->Readyfs->show();
}

void ServerW::on_StartS_clicked()
{
    SVC.startServer(5555);
    ui->sstatus->append("Start server");
}
