#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QElapsedTimer>
#include "./utils/vector2.h"
#include "./gamestate/statemachine.h"
#include "./gamestate/boardstates.h"
#include "./dataresource.h"
#include "./code/windows/modemanager.h"



class BoardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BoardWidget(QWidget *parent = nullptr);


    QSize GetBoardSize() const { return QSize(width_ * cell_size_, height_ * cell_size_); }
    // void update(int delta_time);
    void render(QPainter &painter); // 用于渲染
    void Update();

    bool IsRender() { return is_render_; }
    void SetIsRender(bool is_render) { is_render_ = is_render; }

    QTimer *GetCountDownTimer() { return &count_down_timer_; }

    StateMachine state_machine_;
    void disconnect_board_widget(){
        disconnect(&game_timer_, &QTimer::timeout, this, &BoardWidget::Update);
    }
    void disconnect_counter() {
        disconnect(&count_down_timer_, &QTimer::timeout, this, nullptr);;
    }
protected:
    void mousePressEvent(QMouseEvent *ev) override;    // 由这个类来处理鼠标事件


    void paintEvent(QPaintEvent *event) override{
        Q_UNUSED(event);
        QPainter painter(this);

        if(is_render_) {
            render(painter);
        }

    }


private:
    QTimer game_timer_;
    QTimer count_down_timer_;
    QElapsedTimer elapsed_timer_;

    //背景
    QPixmap board_background_;
    //状态管理

    bool is_render_ = true;
    void InitStateMachine();

    //属性
    int width_;
    int height_;
    int cell_size_;
    QPoint padding_;
    // 维护鼠标事件


    //渲染
    void DrawBK(int start_x, int start_y, int board_width, int board_height, QPainter &painter); // 绘制背景
    void DrawSelect(QPainter &painter);
    // 辅助函数：将像素坐标转换为棋盘坐标
    bool PixelToBoard(int x, int y, Vector2 &pos);
    void StartCounter();
    void SetDifficulty(int choice);
    void SetBoardSize(int choice);
    void SetGameMode(int choice);
public slots:
    void InitData();


};

#endif // BOARDWIDGET_H
