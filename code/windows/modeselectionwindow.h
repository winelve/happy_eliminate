#ifndef ModeSelectionWindow_H
#define ModeSelectionWindow_H

#include <QMainWindow>
#include "framelesswindow.h"
#include "code/audio/audioplayer.h"

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

private:
    Ui::ModeSelectionWindow *ui;
    AudioPlayer* audioPlayer = AudioPlayer::getInstance();
};

#endif // ModeSelectionWindow_H
