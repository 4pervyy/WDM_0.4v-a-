#ifndef ABOUTW_H
#define ABOUTW_H

#include <QDialog>
#include "videop.h"

namespace Ui {
class AboutW;
}

class AboutW : public QDialog
{
    Q_OBJECT

public:
    explicit AboutW(QWidget *parent = nullptr);
    ~AboutW();

private slots:
    void on_VP1_button_clicked();

private:
    Ui::AboutW *ui;
    VideoP *vp;
};

#endif // ABOUTW_H
