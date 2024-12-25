#include "board.h"
#include "./utils/constants.h"
#include "./utils/utils.h"
#include <random>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

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
    board_.clear();
    board_.resize(height, std::vector<std::shared_ptr<Cube>>());

    // 创建并行动画组
    QParallelAnimationGroup* ani_group = new QParallelAnimationGroup(this);
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
                    auto cube = std::make_shared<Cube>(type, Vector2(row, col), this);
                    board_[row].emplace_back(cube);
                    valid = true;

                    // 设置初始位置（在视图之上）

                    QPointF start_pos = Utils::GetRenderPos(Vector2(-(height - row), col));
                    QPointF end_pos = Utils::GetRenderPos(Vector2(row, col));
                    cube->move(start_pos.toPoint());

                    // 创建下落动画
                    QPropertyAnimation* fall_ani = cube->CreatMotionAni(
                        "pos",
                        QVariant::fromValue(start_pos),
                        QVariant::fromValue(end_pos),
                        500,
                        QEasingCurve::InQuad  // 使用弹跳效果让下落更生动
                    );
                    // 将动画添加到动画组
                    ani_group->addAnimation(fall_ani);
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
    ani_group->start(QAbstractAnimation::DeleteWhenStopped);
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
        board_[row][col]->SetRenderPos(Utils::GetRenderPos(row,col));
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
