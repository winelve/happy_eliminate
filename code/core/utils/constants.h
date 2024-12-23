#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QSize>

namespace Constants {

const int k_board_padding = 20;
const int k_cell_size = 64;
const int k_duration_time = 16;
const int ktype_size = 3;
const float move_speed = 400;
const int k_frame_duration = 50;

const int kspecial_cube_row = 1;
const int kspecial_cube_column = 2;
const int kspecial_cube_bomb = 3;
const int kspecial_cube_magic = 4;

const int k_swap_duration = 200;
const QSize k_cube_size = QSize(Constants::k_cell_size - 5,Constants::k_cell_size - 5);

}


#endif // CONSTANTS_H
