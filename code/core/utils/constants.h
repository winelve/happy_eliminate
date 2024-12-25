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

extern int board_width;
extern int board_height;

extern const int k_Max_Time;
extern const int k_Max_Steps;

extern const int k_cube_pace;


extern const int k_price_time;
extern const int k_price_pace;
extern const int k_price_magic;
extern const int k_price_switch;

} // namespace Constants

#endif // CONSTANTS_H
