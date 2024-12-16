#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include "framelesswindow.h"
#include "cvolumesliderdialog.h"
namespace Ui {
class SettingWindow;
}

class SettingWindow : public FrameLessWindow
{
    Q_OBJECT

public:
    explicit SettingWindow(QWidget *parent = nullptr);
    ~SettingWindow();

private slots:
    void on_btnReturn_clicked();       // 返回按钮的点击槽
    void on_btnMusic_clicked();        // 音乐按钮的点击槽
    void on_btnSound_clicked();        // 音效按钮的点击槽
    void onVolumeChanged(int volume);  // 音量变化时的槽函数

private:
    Ui::SettingWindow *ui;
    bool music_status;             // 音乐的状态，true为开启，false为关闭
    bool sound_status;             // 音效的状态，true为开启，false为关闭
    CVolumeSliderDialog *m_volume;       // 音量滑块，用来调整音量
};

#endif // SETTINGWINDOW_H
