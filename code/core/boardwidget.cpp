#include "boardwidget.h"
#include <QPainter>

BoardWidget::BoardWidget(QWidget *parent)
    : QWidget(parent),cell_size_(50), padding_(20)
{
    width_ = 8;
    height_ = 8;

    board_ = new Board(width_,height_);

}




void BoardWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 获取棋盘的尺寸
    int rows = board_->GetHeight();
    int cols = board_->GetWidth();

    // 计算棋盘的总宽度和高度
    int board_width = cols * cell_size_;
    int board_height = rows * cell_size_;

    // 设置棋盘的起始位置（居中或根据需要调整）
    int start_x = padding_;
    int start_y = padding_;

    // 绘制背景（可选）
    painter.setBrush(QBrush(Qt::lightGray));
    painter.drawRect(start_x, start_y, board_width, board_height);

    // 绘制垂直线条
    painter.setPen(QPen(Qt::black, 2));
    for(int col = 0; col <= cols; ++col){
        int x = start_x + col * cell_size_;
        painter.drawLine(x, start_y, x, start_y + board_height);
    }

    // 绘制水平线条
    for(int row = 0; row <= rows; ++row){
        int y = start_y + row * cell_size_;
        painter.drawLine(start_x, y, start_x + board_width, y);
    }

    // 可选：绘制每个 Cube 的类型
    for(int row = 0; row < rows; ++row){
        for(int col = 0; col < cols; ++col){
            Cube cube = board_->GetCube(row, col);
            if(!cube.Empty()){
                cube.paint(painter);
            }
        }
    }
}
