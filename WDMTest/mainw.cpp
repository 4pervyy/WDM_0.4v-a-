#include "mainw.h"
#include "ui_mainw.h"
#include <settingw.h>
#include <QDebug>

QString strMessage = "";
QString resMessage = "";
QChart *chrt;

int vSB_pn, vSB_pt, vSB_ps;




MainW::MainW(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainW)
{
    ui->setupUi(this);
    //Button
    QAction* screen_bt = menuBar()->addAction(tr("Настройки"));
    connect(screen_bt, SIGNAL(triggered(bool)), this, SLOT(settingv()));
    QAction* about_bt = menuBar()->addAction(tr("Справка"));
    connect(about_bt, SIGNAL(triggered(bool)), this, SLOT(aboutv()));
    QAction* exi_bt = menuBar()->addAction(tr("Выход"));
    connect(exi_bt, SIGNAL(triggered(bool)), this, SLOT(closedw()));
    QAction* labwork = menuBar()->addAction(tr("Лаб.работы"));
    connect(labwork, SIGNAL(triggered(bool)), this, SLOT(lab_wp()));

    this->controllerthread = new Controller();
    connect(controllerthread, &Controller::handleResults, this , &MainW::on_sniffer_thread_message);
    connect(controllerthread, &Controller::changeUploadSize, this , &MainW::on_sniffer_thread_new_upload_size);
    connect(controllerthread, &Controller::changeDownloadSize, this , &MainW::on_sniffer_thread_new_download_size);
    connect(controllerthread, &Controller::changeUploadSpeed, this , &MainW::on_sniffer_thread_new_upload_speed);
    connect(controllerthread, &Controller::changeDownloadSpeed, this , &MainW::on_sniffer_thread_new_download_speed);
    QString arg;
    controllerthread->operate(arg);
}

MainW::~MainW()
{
    delete ui;
}



void MainW::on_Button_ping_clicked()
{
      vSB_pn = ui->SB_pn->value();
      vSB_ps = ui->SB_ps->value();
      vSB_pt = ui->SB_pt->value();

      strMessage = "";
      resMessage = "";
        ThreadPing Threadp;
        Threadp.run();
        ui->vchrt->setChart(chrt);
       //qDebug()<<vSB_pn<<vSB_ps<<vSB_pt;
       ui->statusping->setText(strMessage);
       ui->resultping->setText(resMessage);
}


void MainW::vMessage(QString, QString){

}
//ping
void ThreadPing::run(){
     class sender senderd;
     if(senderd.ip().isEmpty()){
         QMessageBox::information(nullptr,"Ошибка","Не указан IP");
         chrt = new QChart;
     }
     else{
    //chart
    chrt = new QChart;
    // graphics link to display element
    chrt->setTitle("График ping");
    // Axis Х
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(1, vSB_pn); // диапазон значений на оси
    axisX->setTickCount(senderd.numberpacket()); // число линий сетки
    axisX->setLabelFormat("%.2f"); // формат отображения чисел на оси
     // Axis Y
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, senderd.maxping());
    axisY->setTickCount(5);
    axisY->setLabelFormat("%g");
    axisX->setTitleText("Номер пакета");
    axisY->setTitleText("Задержка, мс");
    QLineSeries* series1 = new QLineSeries();
    series1->setColor(Qt::darkBlue);
    //series1->setName(tr(" "));



    // Declare variables
    HANDLE hIcmpFile;                       // Handler
    unsigned long ipaddr = INADDR_NONE;     // Destination Address
    DWORD dwRetVal = 0;                     // Number of responses
    char SendData[vSB_ps];      // buffer отсылаемых данных
    LPVOID ReplyBuffer = nullptr;           // buffer answer
    DWORD ReplySize = 0;                    // size buffer answer
    unsigned long timeoutp = vSB_pn; //need fix
    int aresult[vSB_pn];
    int packetinfo[3] = {vSB_pn,0,0};
    int max,min;
    // Set the IP address from the lineEdit field
    ipaddr = inet_addr(senderd.ip().toStdString().c_str());
    hIcmpFile = IcmpCreateFile();   // Create Handler

    // Allocate memory for the response buffer
    ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
    ReplyBuffer = (VOID*) malloc(ReplySize);


         strMessage += "Отправка пакетов на " + senderd.ip() + "\n";
          strMessage += "Количество пакетов: " + QString::number(vSB_pn)+ "\n";
          strMessage += "Ответы: \n" ;

    for(int i=1;i<=vSB_pn;i++) {
        // Call the ICMP echo request function
        dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData),
                    nullptr, ReplyBuffer, ReplySize, timeoutp);
        if (dwRetVal != 0) {
            // Echo response structure
            PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
            struct in_addr ReplyAddr;
            ReplyAddr.S_un.S_addr = pEchoReply->Address;
            // create a line to write response messages

                if (dwRetVal > 1) {
                    strMessage += "Больше ICMP > 1 ";
                }
                    strMessage += "Номер пакета: "+QString::number(i) + "\n";
                    strMessage += "Время = " + QString::number(pEchoReply->RoundTripTime) + " мс | Размер пакета:"+ QString::number(vSB_ps)+" Байт \n";
                    aresult[i-1]=pEchoReply->RoundTripTime;
                    series1->append(i,pEchoReply->RoundTripTime);
                    ++packetinfo[1];
                    //qDebug()<< aresult[i-1];

        } else {
                strMessage += "Номер пакета: "+QString::number(i) + "\n";
                strMessage += "Функция IcmpSendEcho не работает. Пакет потерян. Проверьте настройки \n";
                strMessage += "IcmpSendEcho код ошибки: ";
                strMessage += QString::number(GetLastError()) +"\n";
                ++packetinfo[2];
            }

    }
    chrt->addSeries(series1);
    chrt->setAxisX(axisX, series1);
    chrt->setAxisY(axisY, series1);
     max=aresult[0];
     min=aresult[0];
     for(int i=1;i<=vSB_pn;i++)
     {
         if(max<aresult[i-1]) max=aresult[i-1];
         if(min>aresult[i-1]) min=aresult[i-1];
     }


    resMessage += "Всего пакетов:" + QString::number(packetinfo[0]) + "| Получено пакетов: " + QString::number(packetinfo[1]) +  "| Потеряно пакетов: "  + QString::number(packetinfo[2]) + "\n";
    resMessage += "Максимальное значения: " + QString::number(max) + "| Минимальное значения: " + QString::number(min);


    //wid->uii()->statusping->setText(strMessage);
    //wid->uii()->resultping->setText(resMessage);// Show information about received data
    free(ReplyBuffer); // Memory release
     }
}




//Кнопки
void MainW::settingv(){
    setw = new SettingW(this);
    setw->show();
}
void MainW::aboutv(){
    abw = new AboutW(this);
    abw->show();
}
void MainW::closedw(){
    QApplication::quit();
}
void MainW::lab_wp(){
    vp = new VideoP(this);
    vp->show();
}
void MainW::on_Button_Server_clicked()
{
    aw = new ServerW(this);
    aw->show();
}
void MainW::on_Button_Client_clicked()
{
   clnt = new Client(this);
   clnt->show();
}


void MainW::on_sniffer_thread_message(const QString &s)
{
    ui->tb_ssniffer->append(s);
}

void MainW::on_sniffer_thread_new_upload_size(const QString &s)
{
    ui->label_19->setText(s);
}

void MainW::on_sniffer_thread_new_download_size(const QString &s)
{
    ui->label_21->setText(s);
}

void MainW::on_sniffer_thread_new_upload_speed(const QString &s)
{
    ui->label_20->setText(s);
}

void MainW::on_sniffer_thread_new_download_speed(const QString &s)
{
    ui->label_22->setText(s);
}






void MainW::on_Button_startsniffer_clicked()
{
    controllerthread->worker->abort=false;
}

void MainW::on_button_stopsniffer_clicked()
{
    controllerthread->worker->abort=true;
}
