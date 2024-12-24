#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QSize>
#include <QPoint>

namespace Constants {

// 使用 extern 声明常量
extern QPoint k_board_padding;
extern const int k_cell_size;
extern const int k_duration_time;
extern const int ktype_size;
extern const float move_speed;
extern const int k_frame_duration;

extern const int kspecial_cube_row;
extern const int kspecial_cube_column;
extern const int kspecial_cube_bomb;
extern const int kspecial_cube_magic;

extern const int k_swap_duration;
extern const QSize k_cube_size;

} // namespace Constants

#endif // CONSTANTS_H
