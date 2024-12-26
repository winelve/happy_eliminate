#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "code/core/boardwidget.h"

#include <QWidget>
#include <QTimer>
#include <QElapsedTimer>

namespace Ui {
class GameWidget;
}

class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget();
    void closeStateMachine();

private slots:
    void onLevelChanged(int level);
    void onTimeChanged(int rest_time);
    void onScoreChanged(int score);
    void onTargetScoreChanged(int target_score);
    void onStepChanged(int rest_steps);
    void on_score_button_clicked();
    void on_time_button_clicked();
    void on_magic_button_clicked();
    void on_reset_button_clicked();
    void on_time_stoper_button_clicked();

    void on_setting_button_clicked();
signals:
    void finished(int game_type);

private:
    Ui::GameWidget *ui;
    int game_type_ = 1; //设置的时候使用

    void modifyFont();  // 修改字体
    void RandomMagic(); //随机产生猫咪
    void ResetBoard(); //用于随机打乱棋盘
    void CreateMessageBox(QString message);
};

#endif // GAMEWIDGET_H
