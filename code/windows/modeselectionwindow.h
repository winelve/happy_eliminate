#ifndef ModeSelectionWindow_H
#define ModeSelectionWindow_H

#include <QMainWindow>
#include "framelesswindow.h"
#include "code/audio/audioplayer.h"
#include "modemanager.h"

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

    void on_btnPlay_clicked();

    void on_btnSize_clicked();

    void on_btnDifficulty_clicked();

    void on_btn6_clicked();

    void on_btn10_clicked();

    void on_btn14_clicked();

    void on_btnBreak_clicked();

    void on_btnHeart_clicked();

    void on_btnEasy_clicked();

    void on_btnDifficult_clicked();

    void on_btnExtreme_clicked();

private:
    Ui::ModeSelectionWindow *ui;
    AudioPlayer* audioPlayer = AudioPlayer::getInstance();
    ModeManager* modeManager = ModeManager::getinstance();
    int selectionStatus = 0;
    // int select_checkerboard = 0;    // 选择棋盘大小
    // int select_difficulty = 0;  // 选择游戏难度（棋子种类个数）
    // int select_play = 0;    // 选择玩法
};

#endif // ModeSelectionWindow_H
