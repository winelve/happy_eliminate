#include "board.h"
#include <queue>
#include <random>

const int Board::ktype_size = 6;
Board::Board(const std::vector<std::vector<int>> &board,QObject *parent)
    : QObject{parent}
{
    InitBoard(board);
}

Board::Board(const std::vector<std::vector<Cube>> &board,QObject *parent)
    : QObject{parent}
{
    InitBoard(board);
}

Board::Board(int width,int height,QObject *parent)
    : QObject{parent}
{
    InitRandomBoard(width,height);
}

Board::Board(QObject *parent)
    : QObject{parent}
{
    InitRandomBoard(5,5);
}

// 初始化随机棋盘，确保没有初始消除组合
void Board::InitRandomBoard(int width, int height){
    // 调整 board_ 的大小并初始化为空
    board_.resize(height, std::vector<Cube>(width, Cube()));

    // 使用现代随机数生成器
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, Board::ktype_size); // 假设类型从1开始

    for(int row = 0; row < height; ++row){
        for(int col = 0; col < width; ++col){
            bool valid = false;
            int attempts = 0;
            const int MAX_ATTEMPTS = 100; // 防止无限循环

            while(!valid && attempts < MAX_ATTEMPTS){
                int type = dis(gen);
                if(!CausesMatch(row, col, type)){
                    SetCube(Vector2(row, col), Cube(type));
                    valid = true;
                }
                attempts++;
            }

            if(!valid){
                // 如果经过多次尝试仍未找到合适的类型，强制设置（可能导致匹配）
                int forced_type = (rand() % Board::ktype_size) + 1; // 确保类型范围一致
                SetCube(Vector2(row, col), Cube(forced_type));
                qDebug() << "Warning: Could not find non-matching type for (" << row << "," << col << ")";
            }
        }
    }
}

// 检查当前赋值是否会导致消除组合
bool Board::CausesMatch(int row, int col, int type){
    if(type == 0) return false; // 假设类型0表示空

    int rows = GetHeight();
    int cols = GetWidth();

    // 检查水平
    int match_count = 1;
    // 向左检查
    for(int i = 1; i <= 2; ++i){
        if(col - i < 0) break;
        if(board_[row][col - i].GetType() == type){
            match_count++;
        } else {
            break;
        }
    }
    // 向右检查
    for(int i = 1; i <= 2; ++i){
        if(col + i >= cols) break;
        if(board_[row][col + i].GetType() == type){
            match_count++;
        } else {
            break;
        }
    }
    if(match_count >= 3) return true;

    // 检查垂直
    match_count = 1;
    // 向上检查
    for(int i = 1; i <= 2; ++i){
        if(row - i < 0) break;
        if(board_[row - i][col].GetType() == type){
            match_count++;
        } else {
            break;
        }
    }
    // 向下检查
    for(int i = 1; i <= 2; ++i){
        if(row + i >= rows) break;
        if(board_[row + i][col].GetType() == type){
            match_count++;
        } else {
            break;
        }
    }
    if(match_count >= 3) return true;

    return false;
}





void Board::InitBoard(const std::vector<std::vector<Cube>> &board)
{
    int height = board.size();
    if(height == 0) return;
    int width = board[0].size();

    board_.resize(height, std::vector<Cube>(width, Cube()));

    for(int row = 0; row < height; ++row){
        for(int col = 0; col < width; ++col){
            if(col >= board[row].size()) continue; // 防止不规则输入
            SetCube(Vector2(row, col), board[row][col]);
        }
    }
}


void Board::InitBoard(const std::vector<std::vector<int>> &board)
{
    int row_total = board.size();
    if(row_total == 0) return;
    int col_total = 0;
    for(const auto &row : board){
        if(row.size() > col_total){
            col_total = row.size();
        }
    }

    board_.resize(row_total, std::vector<Cube>(col_total, Cube()));

    for(int row = 0; row < row_total; row++){
        for(int col = 0; col < board[row].size(); col++){
            int type = board[row][col];
            if(type < 0 || type > Board::ktype_size){
                qDebug() << "Board_Warning: Invalid type " << type << " at (" << row << "," << col << ")";
                type = 0; // 默认设置为空或其他处理方式
            }
            SetCube(Vector2(row, col), Cube(type));
        }
    }
}


void Board::PrintBoard(){
    for(auto &list:board_){
        QString tmp;
        int len = 5;
        for(Cube &c:list){
            QString num_str =  QString::number(c.GetType());
            num_str.append(QString(" ").repeated(len-num_str.size()));
            tmp.append(num_str);
        }
        qDebug() << tmp;
    }
}

void Board::DelCube(int row,int col){
    //判断是否有效访问
    if (row < 0 || row >= board_.size() || col < 0 || col >= board_[row].size()) {
        qDebug() << "Out of bounding::In SetBoard.";
    }

    board_[row][col].SetType(0);
    //或许下面还应该执行一些消除后的逻辑
}

void Board::SetCube(Vector2 pos,const Cube &cube){
    int row = pos.GetRow(), col = pos.GetColumn();
    if (row < 0 || row >= board_.size() || col < 0 || col >= board_[row].size()) {
        qDebug() << "Out of bounding::In SetBoard.";
        return ;
    }
    board_[pos.GetRow()][pos.GetColumn()] = cube;
    GetCube(pos).SetPos(pos);
}


// CheckBoard 函数实现
std::vector<std::vector<Vector2>> Board::CheckBoard() {
    std::vector<std::vector<Vector2>> groups; // 存储所有需要被消除的方块分组
    int rows = board_.size();
    if (rows == 0) return groups;
    int cols = board_[0].size();

    // 用于标记需要被消除的方块
    std::vector<std::vector<bool>> to_remove(rows, std::vector<bool>(cols, false));

    // 检测水平匹配
    for (int x = 0; x < rows; ++x) {
        int count = 1;
        for (int y = 1; y < cols; ++y) {
            if (board_[x][y].GetType() != 0 && board_[x][y] == board_[x][y - 1]) {
                count++;
            } else {
                if (count >= 3) {
                    for (int k = y - count; k < y; ++k) {
                        to_remove[x][k] = true;
                    }
                }
                count = 1;
            }
        }
        // 检查行尾
        if (count >= 3) {
            for (int k = cols - count; k < cols; ++k) {
                to_remove[x][k] = true;
            }
        }
    }

    // 检测垂直匹配
    for (int y = 0; y < cols; ++y) {
        int count = 1;
        for (int x = 1; x < rows; ++x) {
            if (board_[x][y].GetType() != 0 && board_[x][y] == board_[x - 1][y]) {
                count++;
            } else {
                if (count >= 3) {
                    for (int k = x - count; k < x; ++k) {
                        to_remove[k][y] = true;
                    }
                }
                count = 1;
            }
        }
        // 检查列尾
        if (count >= 3) {
            for (int k = rows - count; k < rows; ++k) {
                to_remove[k][y] = true;
            }
        }
    }

    // 使用 BFS 分组需要被消除的方块
    std::vector<std::vector<Vector2>> all_coords; // 所有需要消除的坐标分组
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    const std::vector<std::pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

    for (int x = 0; x < rows; ++x) {
        for (int y = 0; y < cols; ++y) {
            if (to_remove[x][y] && !visited[x][y]) {
                std::vector<Vector2> group;
                std::queue<std::pair<int, int>> q;
                q.push({x, y});
                visited[x][y] = true;

                while (!q.empty()) {
                    auto [current_x, current_y] = q.front();
                    q.pop();
                    group.emplace_back(current_x, current_y); // Vector2(row, column)

                    for (const auto& dir : directions) {
                        int new_x = current_x + dir.first;
                        int new_y = current_y + dir.second;
                        if (new_x >= 0 && new_x < rows && new_y >= 0 && new_y < cols &&
                            to_remove[new_x][new_y] && !visited[new_x][new_y]) {
                            q.push({new_x, new_y});
                            visited[new_x][new_y] = true;
                        }
                    }
                }

                if (!group.empty()) {
                    all_coords.push_back(group);
                }
            }
        }
    }

    return all_coords;
}


int Board::ClearCube(const std::vector<std::vector<Vector2>> &cubes_remove) {
    int total_size = 0;

    // 清除
    for(const auto &cube_list : cubes_remove){
        for(const Vector2 &pos : cube_list){
            SetCube(pos, Cube()); // 使用空的 Cube
            total_size++;
        }
    }

    return total_size;
}


void Board::Swap(Vector2 pos_1,Vector2 pos_2){
    Cube cube = GetCube(pos_1);
    SetCube(pos_1,GetCube(pos_2));
    SetCube(pos_2,cube);
}



void Board::Fall() {
    for(int col = 0; col < GetWidth(); ++col){
        int empty_row = GetHeight() - 1; // 从该列的底部开始

        // 从底部向上遍历每一行
        for(int row = GetHeight() - 1; row >= 0; --row){
            if(!GetCube(Vector2(row, col)).Empty()){ // 使用 GetCube
                if(row != empty_row){
                    // 将当前方块下移到空位
                    Cube moving_cube = GetCube(Vector2(row, col));
                    SetCube(Vector2(empty_row, col), moving_cube);
                    // 将原位置设置为空
                    SetCube(Vector2(row, col), Cube()); // 使用空的 Cube
                }
                empty_row--; // 更新下一个空位的位置
            }
        }
    }
}


void Board::Fill(){
    for(int col = 0; col < GetWidth(); ++col){
        // 从底部开始向上遍历每一行
        for(int row = GetHeight() - 1; row >= 0; --row){
            if(GetCube(Vector2(row, col)).Empty()){ // 使用 GetCube
                // 如果当前单元为空，则生成一个新的方块填充
                Cube new_cube = GenerateCube();
                SetCube(Vector2(row, col), new_cube);
            }
        }
    }
}




Cube Board::GenerateCube(){
    int new_type = rand() % Board::ktype_size + 1;
    return Cube(new_type);
}










