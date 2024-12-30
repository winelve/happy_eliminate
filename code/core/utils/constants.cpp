#include "Constants.h"

namespace Constants {

// 定义常量的值
QPoint k_board_padding = QPoint(0,0);
int k_cell_size = 64;
const int k_duration_time = 8;
int ktype_size = 5;
const float move_speed = 400.0f;
const int k_frame_duration = 50;

const int kspecial_cube_row = 1;
const int kspecial_cube_column = 2;
const int kspecial_cube_bomb = 3;
const int kspecial_cube_magic = 4;

const int k_swap_duration = 200;
QSize k_cube_size = QSize(k_cell_size - 5, k_cell_size - 5);


int board_width = 10;
int board_height = 10;

const int k_Max_Time = 10000;
const int k_Max_Steps = 10000;

const int k_cube_pace = 10;

const int k_price_time = 500;
const int k_price_pace = 50;
const int k_price_magic = 0;
const int k_price_switch = 80;

Vector2 Word_Pos = Vector2(board_width/2-1,board_width/2-1);

} // namespace Constants
