#include "cube.h"
#include "code/core/constants.h"
#include "Animation/resourcemanager.h"

#include <QColor>
#include <QPen>
#include <QPainter>
#include <QDebug>

Cube::Cube()
    : type_(0),is_choosed_(false),is_playing_animation_(false)
{
    // cube_texture = ResourceManager::Instance().GetCube(0);
}

Cube::Cube(int type)
    : type_(type),is_choosed_(false),is_playing_animation_(false)
{
    cube_texture = ResourceManager::Instance().GetCube(type);
}


void Cube::paint(QPainter &painter){
    int cell_size = Constants::k_cell_size;
    int padding = Constants::k_board_padding;

    // 计算格子的左上角坐标
    int x = padding + pos_.GetColumn() * cell_size;
    int y = padding + pos_.GetRow() * cell_size;

    // 如果被选中，绘制边框
    if(is_choosed_){
        painter.setPen(QPen(Qt::cyan, 4)); // 选中时的边框颜色和宽度
        painter.drawRect(x, y, cell_size, cell_size);
    }

    // 如果不播放动画，绘制 QPixmap
    if(!is_playing_animation_ && !cube_texture.isNull()){
        // 计算 QPixmap 的缩放尺寸，保持比例并适应格子
        QSize targetSize = QSize(cell_size - 10, cell_size - 10); // 留出一些内边距
        QPixmap scaled_pixmap = cube_texture.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // 计算 QPixmap 的绘制位置，使其居中
        int pix_x = x + (cell_size - scaled_pixmap.width()) / 2;
        int pix_y = y + (cell_size - scaled_pixmap.height()) / 2;

        painter.drawPixmap(pix_x, pix_y, scaled_pixmap);
    }
}


