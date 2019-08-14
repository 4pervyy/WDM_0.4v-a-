#include "aboutw.h"
#include "ui_aboutw.h"


AboutW::AboutW(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutW)
{
    ui->setupUi(this);
}

AboutW::~AboutW()
{
    delete ui;
}

void AboutW::on_VP1_button_clicked()
{
    vp = new VideoP(this);
    vp->show();

}
