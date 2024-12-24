#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "framelesswindow.h"
#include "code/audio/audioplayer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public FrameLessWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnAbout_clicked();

    void on_btnQuit_clicked();

    void on_btnSetting_clicked();

    void on_btnRank_clicked();

    //void on_btnGame_clicked();

public slots:
    void showMainWindow();

private:
    Ui::MainWindow *ui;
    AudioPlayer* audioPlayer = AudioPlayer::getInstance();  //音乐播放器
};

// 延迟时间，为优化切换窗口体验
void delay(int x);

#endif // MAINWINDOW_H
