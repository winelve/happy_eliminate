#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>
#include <QPixmap>
#include "./utils/vector2.h"
#include "./gamestate/statemachine.h"
#include "./gamestate/boardstates.h"



class BoardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BoardWidget(QWidget *parent = nullptr);


    QSize GetBoardSize() const { return QSize(width_ * cell_size_, height_ * cell_size_); }
    // void update(int delta_time);
    void render(QPainter &painter); // 用于渲染
    void Update(int deltatime);
protected:
    void mousePressEvent(QMouseEvent *ev) override;    // 由这个类来处理鼠标事件
private:
    //背景
    QPixmap board_background_;
    //状态管理
    StateMachine state_machine_;
    void InitStateMachine();

    //属性
    int width_;
    int height_;
    int cell_size_;
    int padding_;

    // 维护鼠标事件


    //渲染
    void DrawBK(int start_x, int start_y, int board_width, int board_height, QPainter &painter); // 绘制背景
    void DrawSelect(QPainter &painter);
    // 辅助函数：将像素坐标转换为棋盘坐标
    bool PixelToBoard(int x, int y, Vector2 &pos);
};

#endif // BOARDWIDGET_H
