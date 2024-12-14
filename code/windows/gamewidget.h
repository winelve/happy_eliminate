#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include "code/core/boardwidget.h"
#include "code/core/Animation/animationmanager.h"

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

private:
    Ui::GameWidget *ui;
    BoardWidget board_widget_;

    QTimer game_timer_;
    QElapsedTimer elapsed_timer_;
    AnimationManager *animation_manager_ = AnimationManager::GetInstance();

    void paintEvent(QPaintEvent *event) override;



private slots:
    void onUpdate(); //更新整体的数据


};

#endif // GAMEWIDGET_H
