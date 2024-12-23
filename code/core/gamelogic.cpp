#include "gamelogic.h"
#include "./utils/utils.h"
#include <QDebug>
#include <queue>

// 静态方法，返回单例实例
GameLogic& GameLogic::instance()
{
    static GameLogic instance; // 静态局部变量保证全局唯一性
    return instance;
}

// 私有构造函数
GameLogic::GameLogic(QObject *parent)
    : QObject(parent)
{
    qDebug() << "GameLogic instance created.";
}

std::vector<std::vector<Vector2>> GameLogic::CheckBoard(std::shared_ptr<Board> board) {
    std::vector<std::vector<Vector2>> groups; // 存储所有需要被消除的方块分组
    int rows = board->GetHeight();
    if (rows == 0) return groups;
    int cols = board->GetWidth();

    // 标记需要被消除的方块
    std::vector<std::vector<bool>> to_remove(rows, std::vector<bool>(cols, false));

    // 检测水平匹配
    for (int x = 0; x < rows; ++x) {
        int count = 1;
        for (int y = 1; y < cols; ++y) {
            if (board->GetCube(x, y) && board->GetCube(x, y - 1) &&
                board->GetCube(x, y)->GetType() != 0 &&
                board->GetCube(x, y)->GetType() == board->GetCube(x, y - 1)->GetType()) {
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
            if (board->GetCube(x, y) && board->GetCube(x - 1, y) &&
                board->GetCube(x, y)->GetType() != 0 &&
                board->GetCube(x, y)->GetType() == board->GetCube(x - 1, y)->GetType()) {
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
    std::vector<std::vector<Vector2>> all_coords;
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    const std::vector<std::pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

    for (int x = 0; x < rows; ++x) {
        for (int y = 0; y < cols; ++y) {
            if (to_remove[x][y] && !visited[x][y] && board->GetCube(x, y)) {
                std::vector<Vector2> group;
                std::queue<std::pair<int, int>> q;
                q.push({x, y});
                visited[x][y] = true;

                while (!q.empty()) {
                    auto [current_x, current_y] = q.front();
                    q.pop();
                    group.emplace_back(current_x, current_y);

                    for (const auto& dir : directions) {
                        int new_x = current_x + dir.first;
                        int new_y = current_y + dir.second;
                        if (new_x >= 0 && new_x < rows && new_y >= 0 && new_y < cols &&
                            to_remove[new_x][new_y] && !visited[new_x][new_y] && board->GetCube(new_x, new_y)) {
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


int GameLogic::ClearCube(std::shared_ptr<Board> board, const std::vector<std::vector<Vector2>>& cubes_remove) {
    int total_size = 0;

    for (const auto& cube_list : cubes_remove) {
        for (const Vector2& pos : cube_list) {
            int row = pos.GetRow();
            int col = pos.GetColumn();
            if (board->GetCube(row, col)) {
                board->GetCube(row, col)->SetType(0); // 设置为空
                // board->GetCube(row, col)->SetState(""); // 设置为空
                total_size++;
            }
        }
    }

    return total_size;
}


void GameLogic::Swap(std::shared_ptr<Board> board, const Vector2& pos_1, const Vector2& pos_2) {
    auto cube_1 = board->GetCube(pos_1);
    auto cube_2 = board->GetCube(pos_2);

    if (!cube_1 || !cube_2) {
        qDebug() << "Swap: One of the positions is invalid.";
        return;
    }

    // 通过 SetCube 方法重新设置 Cube
    board->SetCube(pos_1, cube_2);
    board->SetCube(pos_2, cube_1);
}

void GameLogic::Fall(std::shared_ptr<Board> board) {
    for (int col = 0; col < board->GetWidth(); ++col) {
        int empty_row = board->GetHeight() - 1;

        for (int row = board->GetHeight() - 1; row >= 0; --row) {
            if (board->GetCube(row, col) && !board->GetCube(row, col)->Empty()) {
                if (row != empty_row) {
                    auto moving_cube = board->GetCube(row, col);
                    auto empty_cube = board->GetCube(empty_row, col);
                    board->SetCube(empty_row, col, moving_cube);
                    board->SetCube(row, col, nullptr);

                    moving_cube->SetPos(Vector2(empty_row, col));
                    qDebug() << "Falllllllllllllllllll";
                    emit fallEvent(row, col, empty_row, col,moving_cube);
                }
                empty_row--;
            }
        }
    }
}

void GameLogic::Fill(std::shared_ptr<Board> board) {
    for (int col = 0; col < board->GetWidth(); ++col) {
        int empty_above = 0;
        for (int row = board->GetHeight() - 1; row >= 0; --row) {
            if (!board->GetCube(row, col) || board->GetCube(row, col)->Empty()) {
                auto new_cube = Utils::GenerateCube();
                new_cube->SetPos(Vector2(row, col));
                board->SetCube(row, col, new_cube);
                //这边也是掉落动画
                emit fallEvent(-1-empty_above,col,row,col,new_cube);
                empty_above++;
            }else empty_above = 0;
        }

    }
}







