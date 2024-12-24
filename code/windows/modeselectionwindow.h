#ifndef MODESELECTIONWINDOW_H
#define MODESELECTIONWINDOW_H

#include <QMainWindow>
#include "framelesswindow.h"
#include "code/audio/audioplayer.h"
#include "audiocontrol.h"

namespace Ui {
class ModeSelectionWindow;
}

class ModeSelectionWindow : public FrameLessWindow
{
    Q_OBJECT

public:
    explicit ModeSelectionWindow(QWidget *parent = nullptr);
    ~ModeSelectionWindow();

private slots:
    void on_btnReturn_clicked();

    void on_btnMusic_clicked();

    void on_btnSound_clicked();

    void on_music_slider_valueChanged(int value);

    void on_sound_slider_valueChanged(int value);

private:
    Ui::ModeSelectionWindow *ui;
    AudioPlayer* audioPlayer=AudioPlayer::getInstance();
    AudioControl* audioCtrl = AudioControl::instance();

    bool music_status;             // 音乐的状态，true为开启，false为关闭
    bool sound_status;             // 音效的状态，true为开启，false为关闭
};

#endif // MODESELECTIONWINDOW_H
