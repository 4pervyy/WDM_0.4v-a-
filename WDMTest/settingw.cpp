#include "settingw.h"
#include "ui_settingw.h"

//var
QString ips;
int maxp;
int numberp;
qint16 qnp=5555;



SettingW::SettingW(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingW)
{
    ui->setupUi(this);
    //Spell checking
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->ippco->setValidator(ipValidator);


}


int sender::maxping(){
    return maxp;
}

SettingW::~SettingW()
{
    delete ui;
}


QString sender::ip(){
    return ips;
}

quint16 sender::numberport(){
    return qnp;
}

int sender::numberpacket(){
    return numberp;
}

void SettingW::on_pushButton_clicked()
{
    ui->myip->clear();
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
     for(int nIter=0; nIter<list.count(); nIter++)
      {
          if(!list[nIter].isLoopback())
              if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol )
              ui->myip->append(list[nIter].toString());
      }
}
void SettingW::on_SandE_Button_clicked()
{
    ips =  ui->ippco->text();
    maxp = ui->MaxP_sb->value();
    numberp = ui->Numberp_sb->value();
    qnp = ui->sb_port->value();
}
