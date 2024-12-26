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

private slots:
    void onLevelChanged(int level);
    void onTimeChanged(int rest_time);
    void onScoreChanged(int score);
    void onTargetScoreChanged(int target_score);
    void onStepChanged(int rest_steps);

    void on_score_button_clicked();

    void on_time_button_clicked();

    void on_magic_button_clicked();

signals:
    void finished(int game_type);

private:
    Ui::GameWidget *ui;

    int game_type_ = 1;

    void RandomMagic();
    void modifyFont();

};

#endif // GAMEWIDGET_H
