#ifndef SETTINGWINDOW_H
#define SETTINGWINDOW_H

#include "framelesswindow.h"

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
    void on_btnReturn_clicked();

    void on_btnMusic_clicked();

    void on_btnSound_clicked();

private:
    Ui::SettingWindow *ui;
    bool music_status;  // 记录音乐的状态
    bool sound_status;  // 记录音效的状态
};

#endif // SETTINGWINDOW_H
