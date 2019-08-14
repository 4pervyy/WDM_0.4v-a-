#ifndef VIDEOP_H
#define VIDEOP_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QFileDialog>


namespace Ui {
class VideoP;
}

class VideoP : public QMainWindow
{
    Q_OBJECT

public:
    explicit VideoP(QWidget *parent = nullptr);
    ~VideoP();

private slots:
    void on_Button_open_clicked();

    void on_Button_start_clicked();

    void on_Button_pause_clicked();

    void on_Button_stop_clicked();

private:
    Ui::VideoP *ui;
    QMediaPlayer* player;
    QVideoWidget* vw;
};

#endif // VIDEOP_H
