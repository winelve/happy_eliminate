#include "cube.h"
#include "code/core/constants.h"
#include <QColor>
#include <QPen>
#include <QPainter>
#include <QDebug>

Cube::Cube()
    : type_(0){}

Cube::Cube(int type)
    : type_(type){}


void Cube::paint(QPainter &painter){
    int cell_size = Constants::k_cell_size;
    int padding = Constants::k_board_padding;

    // 根据类型设置颜色
    QColor color;
    switch(GetType()){
    case 1: color = Qt::red; break;
    case 2: color = Qt::green; break;
    case 3: color = Qt::blue; break;
    case 4: color = QColor(0,178,238); break;
    case 5: color = Qt::yellow; break;
    case 6: color = QColor(208,32,144); break;
    // 添加更多类型颜色
    default: color = Qt::black; break;
    }

    painter.setBrush(QBrush(color));
    painter.setPen(Qt::NoPen);

    int x = padding + pos_.GetColumn() * cell_size;
    int y = padding + pos_.GetRow() * cell_size;

    // 绘制带内边距的矩形
    painter.drawRect(x + 5, y + 5, cell_size - 10, cell_size - 10); // 内边距

    // 设置绘制文本的颜色和字体
    painter.setPen(Qt::black); // 根据背景颜色选择对比色
    QFont font = painter.font();
    font.setBold(true);
    font.setPointSize(cell_size / 2); // 调整字体大小以适应方块
    painter.setFont(font);

    // 准备要绘制的文本
    QString text = QString::number(GetType());

    // 在方块中心绘制文本
    painter.drawText(x, y, cell_size, cell_size, Qt::AlignCenter, text);
}
