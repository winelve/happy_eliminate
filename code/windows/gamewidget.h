#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "code/core/boardwidget.h"
#include "code/audio/audioplayer.h"
#include <QWidget>
#include <QTimer>

namespace Ui {
class GameWidget;
}

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget();

private:
    Ui::GameWidget *ui;
    BoardWidget board_widget_;
    AudioPlayer* audioPlayer = AudioPlayer::getInstance();
    QTimer game_timer_;

private slots:
    void onUpdate(); //更新整体的数据
    void on_btnReturn_clicked();
};

#endif // GAMEWIDGET_H
