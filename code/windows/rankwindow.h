#ifndef RANKWINDOW_H
#define RANKWINDOW_H

#include <QMainWindow>
#include "framelesswindow.h"
#include "code/audio/audioplayer.h"

namespace Ui {
class RankWindow;
}

class RankWindow : public FrameLessWindow
{
    Q_OBJECT

public:
    explicit RankWindow(QWidget *parent = nullptr);
    ~RankWindow();

private slots:
    void on_btnReturn_clicked();

private:
    Ui::RankWindow *ui;
    AudioPlayer* audioPlayer = AudioPlayer::getInstance();
};

#endif // RANKWINDOW_H
