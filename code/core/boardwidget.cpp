#include "boardwidget.h"
#include "vector2.h"
#include "constants.h"
#include <QPainter>
#include <vector>
#include <QMouseEvent>


BoardWidget::BoardWidget(QWidget *parent)
    : QWidget(parent),click_time(0)
{
    cell_size_ = Constants::k_cell_size;
    padding_ = Constants::k_board_padding;
    width_ = 8;
    height_ = 8;


    board_ = new Board(width_,height_);

    // std::vector<std::vector<int>> matrix =
    // {
    //     {2, 3, 4, 4, 1},
    //     {1, 1, 2, 1, 5},
    //     {3, 6, 1, 2, 6},
    //     {1, 2, 1, 3, 5},
    // };
    // board_ = new Board(matrix);
    // 设置窗口大小
    setFixedSize(GetBoardSize() + QSize(2 * padding_, 2 * padding_));
}




void BoardWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    Draw(painter);
}

void BoardWidget::DrawBK(int start_x,int start_y,int board_width,int board_height,QPainter &painter){
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制背景（可选）
    painter.setBrush(QBrush(Qt::lightGray));
    painter.drawRect(start_x, start_y, board_width, board_height);
}

void BoardWidget::Draw(QPainter &painter){
    // 获取棋盘的尺寸
    int rows = board_->GetHeight();
    int cols = board_->GetWidth();

    // 计算棋盘的总宽度和高度
    int board_width = cols * cell_size_;
    int board_height = rows * cell_size_;

    // 设置棋盘的起始位置（居中或根据需要调整）
    int start_x = padding_;
    int start_y = padding_;

    DrawBK(start_x,start_y,board_width,board_height,painter);

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

void BoardWidget::onUpdate(){
    // board
    update(); //用于重绘
}


// BoardWidget.cpp

void BoardWidget::mousePressEvent(QMouseEvent *ev){
    if(ev->button() == Qt::LeftButton){
        click_time++;

        // 获取鼠标点击的像素坐标
        int x = ev->pos().x();
        int y = ev->pos().y();

        Vector2 board_pos;
        bool valid = PixelToBoard(x, y, board_pos);

        if(valid){
            qDebug() << "Clicked on board position: Row =" << board_pos.GetRow() << ", Column =" << board_pos.GetColumn();

            if(click_time == 1){
                // 记录第一次点击的坐标
                first_pos_ = board_pos;
                board_->GetCube(first_pos_).SetChoosed(true);

                qDebug() << "First position recorded:" << first_pos_.GetRow() << "," << first_pos_.GetColumn()
                         << "\ttype:" << board_->GetCube(first_pos_).GetType();;

            }
            else if(click_time == 2){
                // 记录第二次点击的坐标
                second_pos_ = board_pos;
                qDebug() << "Second position recorded:" << second_pos_.GetRow() << "," << second_pos_.GetColumn();

                // 检查两个位置是否相邻
                if(areAdjacent(first_pos_, second_pos_)){
                    qDebug() << "Positions are adjacent. Proceeding with swap.";
                    board_->RunGameLogic(first_pos_, second_pos_);
                }
                else{
                    qDebug() << "Positions are not adjacent. Cancelling selection.";
                    // 这里可以添加用户反馈，例如显示消息框或其他方式提示用户
                    // QMessageBox::information(this, "Invalid Move", "Selected cubes are not adjacent.");

                    // 取消选择，可以通过重绘来更新边框颜色
                }

                // 重置点击计数
                board_->GetCube(first_pos_).SetChoosed(false);
                click_time = 0;
            }
        }
        else{
            qDebug() << "Clicked outside the board.";
        }

        // 触发重绘（如果需要）
    }

    else if(ev->button() == Qt::RightButton){
        click_time=0;
        board_->GetCube(first_pos_).SetChoosed(false);
        qDebug() << "Right";
    }

    onUpdate();
    // 调用基类的事件处理
    QWidget::mousePressEvent(ev);
}



bool BoardWidget::PixelToBoard(int x, int y, Vector2 &pos) const
{
    // 调整坐标，去除边距
    x -= padding_;
    y -= padding_;

    // 检查点击是否在棋盘区域内
    if (x < 0 || y < 0) return false;

    int row = y / cell_size_;
    int col = x / cell_size_;

    pos.SetColumn(col);
    pos.SetRow(row);

    // 检查行列是否超出棋盘范围
    if (row < 0 || row >= height_ || col < 0 || col >= width_) {
        return false;
    }

    return true;
}

// 辅助函数：检查两个位置是否相邻
bool BoardWidget::areAdjacent(const Vector2 &pos1, const Vector2 &pos2) const {
    int rowDiff = abs(pos1.GetRow() - pos2.GetRow());
    int colDiff = abs(pos1.GetColumn() - pos2.GetColumn());

    // 检查是否在上下左右相邻
    return ( (rowDiff == 1 && colDiff == 0) || (rowDiff == 0 && colDiff == 1) );
}




