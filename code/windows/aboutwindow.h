#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include "framelesswindow.h"
#include "code/audio/audioplayer.h"

namespace Ui {
class AboutWindow;
}

class AboutWindow : public FrameLessWindow
{
    Q_OBJECT

public:
    explicit AboutWindow(QWidget *parent = nullptr);
    ~AboutWindow();

private slots:
    void on_btnReturn_clicked();

private:
    Ui::AboutWindow *ui;
    AudioPlayer* audioPlayer = AudioPlayer::getInstance();
};

#endif // ABOUTWINDOW_H
