#include "gamelogic.h"
#include "./utils/utils.h"
#include "./boardmanager.h"
#include <QDebug>
#include <queue>
#include <unordered_set>

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


// Find4 函数实现
// Find4 函数实现
void GameLogic::Find4(std::shared_ptr<Board> board, std::vector<std::vector<Vector2>> *match) {
    for (std::vector<Vector2> &pos_list : *match) {
        if (pos_list.size() == 4) {
            // Check if the four positions form a horizontal or vertical line
            bool isHorizontal = true;
            bool isVertical = true;

            // Assume the first element as reference
            int baseRow = pos_list[0].GetRow();
            int baseColumn = pos_list[0].GetColumn();

            for (size_t i = 1; i < pos_list.size(); ++i) {
                if (pos_list[i].GetRow() != baseRow) {
                    isHorizontal = false;
                }
                if (pos_list[i].GetColumn() != baseColumn) {
                    isVertical = false;
                }
            }
            bool is_find = isHorizontal || isVertical;

            if (isHorizontal) {
                // Handle horizontal match logic
                qDebug() << ">>>>>>>>>Found a horizontal match of 4 at row <<<<<<<<<<<" << baseRow ;
                board->GetCube(pos_list[0])->SetEliminate(CubeState::Col_Eliminate);
                board->GetCube(pos_list[0])->SetState("col");
            }
            if (isVertical) {
                // Handle vertical match logic
                qDebug() << ">>>>>>>>>>Found a vertical match of 4 at column <<<<<<<<<<," << baseColumn;
                board->GetCube(pos_list[0])->SetEliminate(CubeState::Row_Eliminate);
                board->GetCube(pos_list[0])->SetState("row");
            }
            if(is_find){
                pos_list.erase(pos_list.begin());  // Erase the first element
            }
        }
    }
}


// Find5 函数实现
void GameLogic::Find5(std::shared_ptr<Board> board, std::vector<std::vector<Vector2>> *match) {
    for (std::vector<Vector2> &pos_list : *match) {
        if (pos_list.size() >= 5) {
            // Check if the positions form a horizontal or vertical line
            bool isHorizontal = true;
            bool isVertical = true;

            // Assume the first element as reference
            int baseRow = pos_list[0].GetRow();
            int baseColumn = pos_list[0].GetColumn();

            for (size_t i = 1; i < pos_list.size(); ++i) {
                if (pos_list[i].GetRow() != baseRow) {
                    isHorizontal = false;
                }
                if (pos_list[i].GetColumn() != baseColumn) {
                    isVertical = false;
                }
            }

            bool is_find = isHorizontal || isVertical;
            if(is_find){
                qDebug() << "!!!!!!!!!!Magic!!!!!!!!!";
                board->GetCube(pos_list[0])->SetEliminate(CubeState::Magic_Eliminate);
                board->GetCube(pos_list[0])->SetState("magic");
                board->GetCube(pos_list[0])->SetType(-1);
                pos_list.erase(pos_list.begin());  // Erase the first element
            }

        }
    }
}


void GameLogic::CheckSpecial(std::vector<std::vector<Vector2>> *match) {
    std::shared_ptr<Board> board = BoardManager::instance().GetCurrentBoard();
    std::unordered_set<Vector2> processed; // Use a set to track processed positions
    std::unordered_set<Vector2> all_in_match; // To prevent duplicate additions

    // Initialize all_in_match with existing elements in match
    for (const auto &pos_list : *match) {
        for (const Vector2 &pos : pos_list) {
            all_in_match.insert(pos);
        }
    }

    for (size_t i = 0; i < match->size(); ++i) {
        auto &pos_list = (*match)[i];
        for (const Vector2 &pos : pos_list) {
            // Check if the position has already been processed
            if (processed.count(pos)) {
                continue; // Skip if already processed
            }
            processed.insert(pos); // Mark the position as processed

            // Get the cube at the current position
            auto cube = board->GetCube(pos);
            if (!cube) {
                continue; // Skip if no cube exists at this position
            }

            if (cube->GetEliminate() == CubeState::Col_Eliminate) {
                // Add all elements in the same column to match
                std::vector<Vector2> col_elements;
                int column = pos.GetColumn();
                for (int row = 0; row < board->GetWidth(); ++row) {
                    Vector2 new_pos(row, column);
                    if (!all_in_match.count(new_pos)) { // Only add if not already in match
                        col_elements.emplace_back(new_pos);
                        all_in_match.insert(new_pos); // Mark as added globally
                    }
                }
                if (!col_elements.empty()) {
                    match->push_back(col_elements);
                }
            } else if (cube->GetEliminate() == CubeState::Row_Eliminate) {
                // Add all elements in the same row to match
                std::vector<Vector2> row_elements;
                int row = pos.GetRow();
                for (int column = 0; column < board->GetWidth(); ++column) {
                    Vector2 new_pos(row, column);
                    if (!all_in_match.count(new_pos)) { // Only add if not already in match
                        row_elements.emplace_back(new_pos);
                        all_in_match.insert(new_pos); // Mark as added globally
                    }
                }
                if (!row_elements.empty()) {
                    match->push_back(row_elements);
                }
            }
        }
    }
}

void GameLogic::CheckMagic(int type,std::vector<std::vector<Vector2>> &match,std::shared_ptr<Board> board){
    if (!board) return; // 检查board是否为空，避免空指针问题

    int width = board->GetWidth();
    int height = board->GetHeight();

    // 一个存储所有符合条件的棋子的容器
    std::vector<Vector2> matchingPositions;

    // 遍历board的每个位置
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            // 获取当前位置的棋子
            auto cube = board->GetCube(row, col);
            if (cube && cube->GetType() == type) {
                // 如果棋子的类型与目标类型匹配，则将其位置添加到matchingPositions中
                matchingPositions.emplace_back(row, col);
            }
        }
    }

    // 如果找到了匹配的棋子，将它们作为一个匹配组添加到match
    if (!matchingPositions.empty()) {
        match.push_back(matchingPositions);
    }
}
























