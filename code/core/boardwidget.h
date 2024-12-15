#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <memory> // 引入智能指针

#include "board.h"
#include "Animation/animationmanager.h"

class BoardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BoardWidget(QWidget *parent = nullptr);

    QSize GetBoardSize() const { return QSize(width_ * cell_size_, height_ * cell_size_); }
    void onUpdate(int delta_time);
    void Draw(QPainter &painter) const; // 用于渲染

protected:
    void mousePressEvent(QMouseEvent *ev) override;

private:
    std::shared_ptr<Board> board_; // 使用智能指针管理 Board 对象
    QPixmap board_background_;
    int width_;
    int height_;
    int cell_size_;
    int padding_;   // 边距
    int click_time; // 测试点击

    Vector2 first_pos_;
    Vector2 second_pos_;

    void DrawBK(int start_x, int start_y, int board_width, int board_height, QPainter &painter) const; // 绘制背景

    // 辅助函数：将像素坐标转换为棋盘坐标
    bool PixelToBoard(int x, int y, Vector2 &pos) const;
    // 辅助函数：检查两个位置是否相邻
    bool areAdjacent(const Vector2 &pos1, const Vector2 &pos2) const;
};

#endif // BOARDWIDGET_H
