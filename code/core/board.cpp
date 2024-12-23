#include "board.h"
#include "./utils/constants.h"
#include <random>

// 构造函数
Board::Board(const std::vector<std::vector<int>> &board, QWidget *parent)
    : QWidget{parent}
{
    InitBoard(board);
}

// Board::Board(const std::vector<std::vector<Cube>> &board, QObject *parent)
//     : QObject{parent}
// {
//     InitBoard(board);
// }

Board::Board(int width, int height, QWidget *parent)
    : QWidget{parent}
{
    qDebug() << "Init::before";
    InitRandomBoard(width, height);
    qDebug() << "Init::after";

}

Board::Board(QWidget *parent)
    : QWidget{parent}
{
    InitRandomBoard(5, 5);
}

// 初始化随机棋盘，确保没有初始消除组合
void Board::InitRandomBoard(int width, int height){
    board_.resize(height, std::vector<std::shared_ptr<Cube>>());

    // 使用现代随机数生成器
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, Constants::ktype_size); // 假设类型从1开始

    for(int row = 0; row < height; ++row){
        for(int col = 0; col < width; ++col){
            bool valid = false;
            int attempts = 0;
            const int MAX_ATTEMPTS = 100; // 防止无限循环

            while(!valid && attempts < MAX_ATTEMPTS){
                int type = dis(gen);
                if(!CausesMatch(row, col, type)){
                    board_[row].emplace_back(std::make_shared<Cube>(type, Vector2(row, col),this));
                    valid = true;
                }
                attempts++;
            }

            if(!valid){
                // 如果经过多次尝试仍未找到合适的类型，强制设置（可能导致匹配）
                int forced_type = (rand() % Constants::ktype_size) + 1; // 确保类型范围一致
                board_[row].emplace_back(std::make_shared<Cube>(forced_type, Vector2(row, col)));
                qDebug() << "Warning: Could not find non-matching type for (" << row << "," << col << ")";
            }
        }
    }
}

// // 从 Cube 对象初始化棋盘
// void Board::InitBoard(const std::vector<std::vector<Cube>> &board)
// {
//     int height = board.size();
//     if(height == 0) return;
//     // int width = board[0].size();

//     board_.resize(height, std::vector<std::shared_ptr<Cube>>());

//     for(int row = 0; row < height; ++row){
//         for(int col = 0; col < board[row].size(); col++){
//             std::shared_ptr<Cube> cube = std::make_shared<Cube>(board[row][col]);
//             cube->SetPos(Vector2(row, col));
//             board_[row].emplace_back(cube);
//         }
//     }
// }

// 从整数类型初始化棋盘
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

    board_.resize(row_total, std::vector<std::shared_ptr<Cube>>());

    // 使用现代随机数生成器
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, Constants::ktype_size);

    for(int row = 0; row < row_total; row++){
        for(int col = 0; col < col_total; col++){
            if(col >= board[row].size()){
                // 如果输入棋盘不规则，填充随机方块
                int type = dis(gen);
                board_[row].emplace_back(std::make_shared<Cube>(type, Vector2(row, col)));
                continue;
            }
            int type = board[row][col];
            if(type < 0 || type > Constants::ktype_size){
                qDebug() << "Board_Warning: Invalid type " << type << " at (" << row << "," << col << ")";
                type = 0; // 默认设置为空或其他处理方式
            }
            board_[row].emplace_back(std::make_shared<Cube>(type, Vector2(row, col)));
        }
    }
}

bool Board::CausesMatch(int row, int col, int type){
    if(type == 0) return false; // 假设类型0表示空
    // 检查水平 - 仅向左检查
    int match_count = 1;
    for(int i = 1; i <= 2; ++i){
        if(col - i < 0) break;
        if(board_[row].size() <= static_cast<size_t>(col - i)) break; // 确保列存在
        std::shared_ptr<Cube> left_cube = board_[row][col - i];
        if(left_cube && left_cube->GetType() == type){
            match_count++;
        } else {
            break;
        }
    }
    if(match_count >= 3) return true;

    // 检查垂直 - 仅向上检查
    match_count = 1;
    for(int i = 1; i <= 2; ++i){
        if(row - i < 0) break;
        if(board_.size() <= static_cast<size_t>(row - i)) break; // 确保行存在
        if(board_[row - i].size() <= static_cast<size_t>(col)) break; // 确保列存在
        std::shared_ptr<Cube> up_cube = board_[row - i][col];
        if(up_cube && up_cube->GetType() == type){
            match_count++;
        } else {
            break;
        }
    }
    if(match_count >= 3) return true;

    return false;
}

// 打印棋盘到调试输出
void Board::PrintBoard(){
    for(const auto &row : board_){
        QString tmp;
        int len = 5;
        for(const auto &c : row){
            QString num_str = QString::number(c->GetType());
            num_str.append(QString(" ").repeated(len - num_str.size()));
            tmp.append(num_str);


        }
        qDebug() << tmp;
    }
}

// 删除指定位置的方块
void Board::DelCube(int row, int col){
    if (row < 0 || row >= GetHeight() || col < 0 || col >= GetWidth()) {
        qDebug() << "Out of bounds::In DelCube.";
        return;
    }

    if(board_[row][col]){
        board_[row][col]->SetType(0);
        // 可能还需要执行一些消除后的逻辑
    }
}

// 设置指定位置的方块
void Board::SetCube(Vector2 pos, std::shared_ptr<Cube> cube){
    int row = pos.GetRow(), col = pos.GetColumn();
    if (row < 0 || row >= GetHeight() || col < 0 || col >= GetWidth()) {
        qDebug() << "Out of bounds::In SetCube.";
        return;
    }
    board_[row][col] = cube;
    if(board_[row][col]){
        board_[row][col]->SetPos(pos);
    }
}

// 设置指定位置的方块
void Board::SetCube(int row,int col,std::shared_ptr<Cube> cube){
    if (row < 0 || row >= GetHeight() || col < 0 || col >= GetWidth()) {
        qDebug() << "Out of bounds::In SetCube.";
        return;
    }
    board_[row][col] = cube;
    if(board_[row][col]){
        board_[row][col]->SetPos(Vector2(row,col));
    }
}


// 获取 Cube（通过行和列）
std::shared_ptr<Cube> Board::GetCube(int row, int col) {
    if (row < 0 || row >= GetHeight() || col < 0 || col >= GetWidth()) {
        qDebug() << "GetCube: Position out of bounds (" << row << "," << col << ")";
        return nullptr;
    }
    return board_[row][col];
}

// 获取 Cube（通过 Vector2）
std::shared_ptr<Cube> Board::GetCube(Vector2 pos) {
    return GetCube(pos.GetRow(), pos.GetColumn());
}














// // 运行游戏逻辑：交换两个方块并处理消除
// void Board::RunGameLogic(const Vector2 &pos_1, const Vector2 &pos_2) {

//     if(currentState_==State::WaitingForInput){
//         qDebug() << "运行游戏逻辑，交换位置：("
//                  << pos_1.GetRow() << "," << pos_1.GetColumn()
//                  << ") 与 (" << pos_2.GetRow() << "," << pos_2.GetColumn() << ")";
//         //
//         emit userSelectedTwoPieces(pos_1, pos_2);
//     }

// }



// std::vector<std::vector<Vector2>> Board::find_row_line4(const std::vector<std::vector<Vector2>> &matches) {
//     std::vector<std::vector<Vector2>> row_line;

//     for (const auto &group : matches) {
//         if (group.size() >= 4) { // 至少包含 4 个方块
//             // 检查是否所有元素都在同一行
//             int row = group[0].GetRow();
//             bool sameRow = std::all_of(group.begin(), group.end(), [row](const Vector2 &pos) {
//                 return pos.GetRow() == row;
//             });

//             if (!sameRow) continue; // 如果不在同一行，跳过

//             // 提取所有的列号并排序
//             std::vector<int> cols;
//             for (const auto &pos : group) {
//                 cols.push_back(pos.GetColumn());
//             }
//             std::sort(cols.begin(), cols.end());

//             // 检查是否存在至少 4 个连续的列号
//             int consecutiveCount = 1;
//             for (size_t i = 1; i < cols.size(); ++i) {
//                 if (cols[i] == cols[i - 1] + 1) {
//                     ++consecutiveCount;
//                     if (consecutiveCount >= 4) {
//                         row_line.push_back(group);
//                         break; // 找到符合条件的组合，跳出循环
//                     }
//                 } else {
//                     consecutiveCount = 1; // 重置计数器
//                 }
//             }
//         }
//     }

//     return row_line;
// }


// std::vector<std::vector<Vector2>> Board::find_col_line4(const std::vector<std::vector<Vector2>> &matches) {
//     std::vector<std::vector<Vector2>> col_line;

//     for (const auto &group : matches) {
//         if (group.size() >= 4) { // 至少包含 4 个方块
//             // 检查是否所有元素都在同一列
//             int col = group[0].GetColumn();
//             bool sameColumn = std::all_of(group.begin(), group.end(), [col](const Vector2 &pos) {
//                 return pos.GetColumn() == col;
//             });

//             if (!sameColumn) continue; // 如果不在同一列，跳过

//             // 提取所有的行号并排序
//             std::vector<int> rows;
//             for (const auto &pos : group) {
//                 rows.push_back(pos.GetRow());
//             }
//             std::sort(rows.begin(), rows.end());

//             // 检查是否存在至少 4 个连续的行号
//             int consecutiveCount = 1;
//             for (size_t i = 1; i < rows.size(); ++i) {
//                 if (rows[i] == rows[i - 1] + 1) {
//                     ++consecutiveCount;
//                     if (consecutiveCount >= 4) {
//                         col_line.push_back(group);
//                         break; // 找到符合条件的组合，跳出循环
//                     }
//                 } else {
//                     consecutiveCount = 1; // 重置计数器
//                 }
//             }
//         }
//     }

//     return col_line;
// }

// void Board::RemoveFromMatch(const Vector2 &pos, std::vector<std::vector<Vector2>> &matches) {
//     for (auto it = matches.begin(); it != matches.end(); /* no increment here */) {
//         auto &cube_list = *it;

//         // 删除子向量中的 pos
//         auto pos_it = std::remove(cube_list.begin(), cube_list.end(), pos);
//         if (pos_it != cube_list.end()) {
//             cube_list.erase(pos_it, cube_list.end()); // 真正删除元素
//         }

//         // 如果子向量为空，删除该子向量
//         if (cube_list.empty()) {
//             it = matches.erase(it); // 删除子向量，返回下一个有效迭代器
//         } else {
//             ++it; // 否则，继续检查下一个子向量
//         }
//     }
// }


// void Board::SetRow4(std::vector<std::vector<Vector2>> &matches){
//     std::vector<std::vector<Vector2>> row4_list = find_row_line4(matches);
//     // for(auto &cube_list:row4_list){
//     //     for(Vector2& cube_pos:cube_list ){
//     //         auto it = std::find(falling_cubes_.begin(), falling_cubes_.end(), cube_pos);
//     //         if (it != falling_cubes_.end()) {
//     //             // 找到了
//     //             qDebug() << "row找到了";
//     //             GetCube(*it)->SetSpecialType(Constants::kspecial_cube_column);
//     //             RemoveFromMatch(*it,matches);
//     //             //播放升级动画
//     //         } else{
//     //             GetCube(cube_list[0])->SetSpecialType(Constants::kspecial_cube_column);
//     //             RemoveFromMatch(cube_list[0],matches);
//     //         }
//     //     }
//     // }
//     if(!row4_list.empty()){
//         std::shared_ptr<Cube> cube = GetCube(row4_list[0][0]);
//         cube->SetSpecialType(Constants::kspecial_cube_column);
//         // RemoveFromMatch(cube->GetPos(),matches);
//         // ani_manager_->AddAnimation(ani_factory_.MakeCubeAnimation(cube,GetRenderPos(cube->GetPos()),AnimationType::Col,true),AnimType::Extra);
//     }

// }

// void Board::SetCol4(std::vector<std::vector<Vector2>> &matches){
//     std::vector<std::vector<Vector2>> col4_list = find_col_line4(matches);
//     // for(auto &cube_list:col4_list){
//     //     for(Vector2& cube_pos:cube_list ){
//     //         auto it = std::find(falling_cubes_.begin(), falling_cubes_.end(), cube_pos);
//     //         if (it != falling_cubes_.end()) {
//     //             // 找到了
//     //             qDebug() << "col找到了";
//     //             GetCube(*it)->SetSpecialType(Constants::kspecial_cube_row);
//     //             RemoveFromMatch(*it,matches);
//     //             //播放升级动画
//     //         }
//     //         else{
//     //             GetCube(cube_list[0])->SetSpecialType(Constants::kspecial_cube_row);
//     //             RemoveFromMatch(cube_list[0],matches);
//     //         }
//     //     }
//     // }
//     if(!col4_list.empty()){
//         std::shared_ptr<Cube> cube = GetCube(col4_list[0][0]);
//         cube->SetSpecialType(Constants::kspecial_cube_row);
//         // RemoveFromMatch(cube->GetPos(),matches);
//         // ani_manager_->AddAnimation(ani_factory_.MakeCubeAnimation(cube,GetRenderPos(cube->GetPos()),AnimationType::Row,true),AnimType::Extra);
//     }

// }


// std::vector<std::vector<Vector2>> Board::ProcessSpecialMatches(const std::vector<std::vector<Vector2>> &matches) {
//     std::set<std::pair<int, int>> processed; // 记录已处理的方块，避免重复
//     std::vector<std::vector<Vector2>> result; // 最终的结果

//     // 辅助函数：将方块加入结果并递归检查
//     auto processCube = [&](const Vector2 &pos, auto &&processCubeRef) -> void {
//         int row = pos.GetRow();
//         int col = pos.GetColumn();

//         // 如果已经处理过，直接返回
//         if (!processed.insert({row, col}).second) return;

//         // 获取当前方块
//         if (!board_[row][col]) return;
//         std::shared_ptr<Cube> cube = board_[row][col];

//         // 添加当前方块到结果
//         if (result.empty() || std::find(result.back().begin(), result.back().end(), pos) == result.back().end()) {
//             result.back().push_back(pos);
//         }

//         // 根据特殊类型处理
//         int special_type = cube->GetSpecialType();
//         if (special_type == Constants::kspecial_cube_row) {
//             // 清除整行
//             for (int c = 0; c < GetWidth(); ++c) {
//                 processCubeRef(Vector2(row, c), processCubeRef);
//             }
//             // std::shared_ptr<MoveAnimation> ani = ani_factory_.MakeMoveAnimation(
//             //     cube,
//             //     ResourceManager::Instance().GetHLine(),
//             //     GetRenderPos(cube->GetPos()),
//             //     GetRenderPos(Vector2(cube->GetPos().GetRow(),-10)),
//             //     cube->GetType(),
//             //     false,
//             //     1500,
//             //     false);
//             // ani_manager_->AddAnimation(ani,AnimType::Extra);

//         } else if (special_type == Constants::kspecial_cube_column) {
//             // 清除整列
//             for (int r = 0; r < GetHeight(); ++r) {
//                 processCubeRef(Vector2(r, col), processCubeRef);
//             }
//         }
//     };

//     // 遍历初始 matches
//     for (const auto &group : matches) {
//         result.emplace_back(); // 开始一个新的组
//         for (const auto &pos : group) {
//             processCube(pos, processCube); // 处理当前方块
//         }
//     }

//     return result;
// }

















