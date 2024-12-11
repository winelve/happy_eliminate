#include "board.h"
#include <queue>


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

void Board::InitBoard(const std::vector<std::vector<Cube>> &board)
{
    board_ = board;
}

void Board::InitBoard(const std::vector<std::vector<int>> &board)
{
    int row_total = board.size();
    board_.resize(row_total);

    for(size_t row=0;row<board.size();row++){
        for(size_t col=0;col<board[row].size();col++){
            board_[row].resize(board[row].size());
            board_[row][col].SetType(board[row][col]);
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

void Board::SetBoard(int row,int col,int type){
    //判断是否有效访问
    if (row < 0 || row >= board_.size() || col < 0 || col >= board_[row].size()) {
        qDebug() << "Out of bounding::In SetBoard.";
    }

    board_[row][col].SetType(type);
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


int Board::ClearCube(std::vector<std::vector<Vector2>> cubes_remove)
{
    int total_size = 0;

    //清除
    for(auto &cube_list: cubes_remove){
        for(const Vector2 &pos:cube_list){
            SetBoard(pos.GetRow(),pos.GetColumn(),0);
        }
    }

    return total_size;
}


void Board::Fall(){

}











