#include "board.h"
#include <queue>
#include <random>
#include <algorithm>
#include "constants.h"

// 构造函数
Board::Board(const std::vector<std::vector<int>> &board, QObject *parent)
    : QObject{parent}
{
    InitBoard(board);
    InitializeFSM();
}

Board::Board(const std::vector<std::vector<Cube>> &board, QObject *parent)
    : QObject{parent}
{
    InitBoard(board);
    InitializeFSM();
}

Board::Board(int width, int height, QObject *parent)
    : QObject{parent}
{
    qDebug() << "Init::before";
    InitRandomBoard(width, height);
    qDebug() << "Init::after";
    InitializeFSM();
}

Board::Board(QObject *parent)
    : QObject{parent}
{
    InitRandomBoard(5, 5);
    InitializeFSM();
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
                    board_[row].emplace_back(std::make_shared<Cube>(type, Vector2(row, col)));
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


// 从 Cube 对象初始化棋盘
void Board::InitBoard(const std::vector<std::vector<Cube>> &board)
{
    int height = board.size();
    if(height == 0) return;
    int width = board[0].size();

    board_.resize(height, std::vector<std::shared_ptr<Cube>>());

    for(int row = 0; row < height; ++row){
        for(int col = 0; col < board[row].size(); col++){
            std::shared_ptr<Cube> cube = std::make_shared<Cube>(board[row][col]);
            cube->SetPos(Vector2(row, col));
            board_[row].emplace_back(cube);
        }
    }
}

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

    int rows = GetHeight();
    int cols = GetWidth();

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


// 新增的 GetRenderPosition 函数实现，返回 QPointF 以提高精度
QPointF Board::GetRenderPos(Vector2 pos) {
    qreal cell_size = static_cast<qreal>(Constants::k_cell_size);
    qreal padding = static_cast<qreal>(Constants::k_board_padding);

    qreal x = padding + pos.GetColumn() * cell_size;
    qreal y = padding + pos.GetRow() * cell_size;

    return QPointF(x, y);
}

// 新增的 GetRenderPosition 函数实现，返回 QPointF 以提高精度
QPointF Board::GetRenderPos(int row, int col) {
    qreal cell_size = static_cast<qreal>(Constants::k_cell_size);
    qreal padding = static_cast<qreal>(Constants::k_board_padding);

    qreal x = padding + col * cell_size;
    qreal y = padding + row * cell_size;

    return QPointF(x, y);
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

// 检查棋盘是否有匹配
std::vector<std::vector<Vector2>> Board::CheckBoard() {
    std::vector<std::vector<Vector2>> groups; // 存储所有需要被消除的方块分组
    int rows = GetHeight();
    if (rows == 0) return groups;
    int cols = GetWidth();

    // 标记需要被消除的方块
    std::vector<std::vector<bool>> to_remove(rows, std::vector<bool>(cols, false));

    // 检测水平匹配
    for (int x = 0; x < rows; ++x) {
        int count = 1;
        for (int y = 1; y < cols; ++y) {
            if (board_[x][y] && board_[x][y - 1] &&
                board_[x][y]->GetType() != 0 &&
                board_[x][y]->GetType() == board_[x][y - 1]->GetType()) {
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
            if (board_[x][y] && board_[x - 1][y] &&
                board_[x][y]->GetType() != 0 &&
                board_[x][y]->GetType() == board_[x - 1][y]->GetType()) {
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
            if (to_remove[x][y] && !visited[x][y] && board_[x][y]) {
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
                            to_remove[new_x][new_y] && !visited[new_x][new_y] && board_[new_x][new_y]) {
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

// 清除指定的方块
int Board::ClearCube(const std::vector<std::vector<Vector2>> &cubes_remove) {
    int total_size = 0;

    // 清除方块
    for(const auto &cube_list : cubes_remove){
        for(const Vector2 &pos : cube_list){
            int row = pos.GetRow();
            int col = pos.GetColumn();
            if(board_[row][col]){
                board_[row][col]->SetType(0); // 设置为空
                total_size++;
            }
        }
    }

    return total_size;
}

// 交换棋盘上的两个方块
void Board::Swap(const Vector2 &pos_1, const Vector2 &pos_2){
    std::shared_ptr<Cube> cube_1 = GetCube(pos_1);
    std::shared_ptr<Cube> cube_2 = GetCube(pos_2);

    if(!cube_1 || !cube_2){
        qDebug() << "Swap: One of the positions is invalid.";
        return;
    }

    std::swap(board_[pos_1.GetRow()][pos_1.GetColumn()], board_[pos_2.GetRow()][pos_2.GetColumn()]);

    // 更新位置
    if(board_[pos_1.GetRow()][pos_1.GetColumn()]){
        board_[pos_1.GetRow()][pos_1.GetColumn()]->SetPos(pos_1);
    }
    if(board_[pos_2.GetRow()][pos_2.GetColumn()]){
        board_[pos_2.GetRow()][pos_2.GetColumn()]->SetPos(pos_2);
    }
}

// 处理下落的方块
void Board::Fall() {
    for(int col = 0; col < GetWidth(); ++col){
        int empty_row = GetHeight() - 1; // 从该列的底部开始

        // 从底部向上遍历每一行
        for(int row = GetHeight() - 1; row >= 0; --row){
            if(board_[row][col] && !board_[row][col]->Empty()){
                if(row != empty_row){
                    // 将当前方块下移到空位
                    std::shared_ptr<Cube> moving_cube = board_[row][col];
                    board_[row][col] = nullptr;

                    board_[empty_row][col] = moving_cube;
                    board_[empty_row][col]->SetPos(Vector2(empty_row, col));
                    // falling_cubes_.push_back(moving_cube->GetPos());

                    // 创建移动动画
                    QPointF start_1 = GetRenderPos(row, col);
                    QPointF end_1 = GetRenderPos(empty_row, col);
                    ani_manager_->AddAnimation(ani_factory_.MakeMoveAnimation(moving_cube, start_1, end_1, board_[empty_row][col]->GetType(),true) ,AnimType::Fall);
                    board_[empty_row][col]->SetPlaying(true);
                }
                empty_row--; // 更新下一个空位的位置
            }
        }
    }
}

// 填充空位
void Board::Fill(){
    for(int col = 0; col < GetWidth(); ++col){
        int empty_above = 0; // 当前列中尚未填充的空位数量
        for(int row = GetHeight() - 1; row >= 0; --row){
            if(!board_[row][col] || board_[row][col]->Empty()){
                // 生成一个新的方块并放置
                std::shared_ptr<Cube> new_cube = GenerateCube();
                new_cube->SetPos(Vector2(row, col));
                board_[row][col] = new_cube;

                // 根据空位数量计算起始位置
                // 每多一个空位，起始位置向上移动一个单元格高度
                QPointF start_1 = GetRenderPos(-1 - empty_above, col);
                QPointF end_1 = GetRenderPos(row, col);

                // 添加动画
                ani_manager_->AddAnimation(
                    ani_factory_.MakeMoveAnimation(new_cube, start_1, end_1, new_cube->GetType(),true),
                    AnimType::Fall);
                //添加到新增数组
                // falling_cubes_.push_back(new_cube->GetPos());

                empty_above++; // 增加空位数量
            }
            else{
                // 如果当前位非空，重置空位计数
                empty_above = 0;
            }
        }
    }
}


void Board::HandleMouseClick(const Vector2 &pos) {
    qDebug() << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! State: " << choose_one_;
    // Only handle clicks when the game is in the WaitingForInput state
    if (currentState_ != State::WaitingForInput) {
        qDebug() << "HandleMouseClick: Ignored click because current state is not WaitingForInput.";
        return;
    }

    // If no cube is currently selected, record the first click
    if (!choose_one_) {
        first_click_pos_ = pos;
        choose_one_ = true;
        GetCube(first_click_pos_)->SetChoosed(true);
        qDebug() << "HandleMouseClick: First cube selected at ("
                 << first_click_pos_.GetRow() << ", " << first_click_pos_.GetColumn() << ").";
    }
    else {
        // Second cube selection
        second_click_pos_ = pos;
        qDebug() << "HandleMouseClick: Second cube selected at ("
                 << second_click_pos_.GetRow() << ", " << second_click_pos_.GetColumn() << ").";

        // Check if the second click is on the same cube as the first
        if (first_click_pos_.GetRow() == second_click_pos_.GetRow() &&
            first_click_pos_.GetColumn() == second_click_pos_.GetColumn()) {
            qDebug() << "HandleMouseClick: Second click is the same as the first. Selection reset.";
            ResetSelection();
            return;
        }

        // Check if the two selected cubes are adjacent
        if (areAdjacent(first_click_pos_, second_click_pos_)) {
            qDebug() << "HandleMouseClick: Selected cubes are adjacent. Initiating swap.";
            GetCube(first_click_pos_)->SetChoosed(false);
            choose_one_ = false;
            emit userSelectedTwoPieces(first_click_pos_,second_click_pos_);
        }
        else {
            qDebug() << "HandleMouseClick: Selected cubes are not adjacent. Selection reset.";
            ResetSelection();
        }
    }
}

void Board::ResetSelection()
{
    if(first_click_pos_.GetRow() > -1 || first_click_pos_.GetColumn() > -1){
        std::shared_ptr<Cube> first_cube = GetCube(first_click_pos_);
        if(first_cube){
            first_cube->SetChoosed(false);
        }
        first_click_pos_ = Vector2(); // 重置为无效状态
    }
    second_click_pos_ = Vector2();
    qDebug() << "选择状态已重置。";
    choose_one_ = false;
}

bool Board::areAdjacent(const Vector2 &pos1, const Vector2 &pos2) const
{
    int rowDiff = abs(pos1.GetRow() - pos2.GetRow());
    int colDiff = abs(pos1.GetColumn() - pos2.GetColumn());

    // 检查是否在上下左右相邻
    return ( (rowDiff == 1 && colDiff == 0) || (rowDiff == 0 && colDiff == 1) );
}

// 生成一个新的随机方块
std::shared_ptr<Cube> Board::GenerateCube(){
    int new_type = (rand() % Constants::ktype_size) + 1;
    return std::make_shared<Cube>(new_type);
}

// 运行游戏逻辑：交换两个方块并处理消除
void Board::RunGameLogic(const Vector2 &pos_1, const Vector2 &pos_2) {

    if(currentState_==State::WaitingForInput){
        qDebug() << "运行游戏逻辑，交换位置：("
                 << pos_1.GetRow() << "," << pos_1.GetColumn()
                 << ") 与 (" << pos_2.GetRow() << "," << pos_2.GetColumn() << ")";
        //
        emit userSelectedTwoPieces(pos_1, pos_2);
    }

}

// 初始化状态机
void Board::InitializeFSM() {
    setupConnections();
    transitionTo(State::WaitingForInput);
}

// 设置状态机的信号槽连接
void Board::setupConnections() {
    // 连接 AnimationManager 的完成信号到 Board 的状态转换
    AnimationManager* anim_manager = AnimationManager::GetInstance();

    // 连接用户选择两个棋子的信号到槽函数
    connect(this, &Board::userSelectedTwoPieces, this, [&](const Vector2 &pos1, const Vector2 &pos2){
        first_pos_ = pos1;
        second_pos_ = pos2;
        transitionTo(State::Swapping);
    });

    // 交换动画完成信号
    connect(anim_manager, &AnimationManager::swapAnimationsFinished, this, [&](){
        transitionTo(State::CheckingMatch);
    });

    // 检查匹配结果
    connect(this, &Board::matchFound, this, [&](){
        transitionTo(State::Clearing);
    });

    connect(this, &Board::noMatch, this, [&](){
        // 如果没有匹配，交换回去
        Swap(first_pos_, second_pos_);
        transitionTo(State::WaitingForInput);
    });

    // 清除动画完成信号
    connect(anim_manager, &AnimationManager::clearAnimationsFinished, this, [&](){
        transitionTo(State::Falling);
    });

    // 下落动画完成信号
    connect(anim_manager, &AnimationManager::fallAnimationsFinished, this, [&](){
        transitionTo(State::EndCheck);
    });

    // 结束检查后是否有新的匹配
    connect(this, &Board::newMatchFound, this, [&](){
        transitionTo(State::CheckingMatch);
    });

    connect(this, &Board::noNewMatch, this, [&](){
        transitionTo(State::WaitingForInput);
    });
}

// 处理状态转换
void Board::transitionTo(State newState) {
    if (currentState_ == newState) return;
    currentState_ = newState;
    qDebug() << "Target::状态转换：" << static_cast<int>(currentState_) << " -> " << static_cast<int>(newState);

    // 调用对应状态的进入函数
    switch (currentState_) {
    case State::WaitingForInput:
        enterWaitingForInput();
        break;
    case State::Swapping:
        enterSwapping();
        break;
    case State::CheckingMatch:
        enterCheckingMatch();
        break;
    case State::Clearing:
        enterClearing();
        break;
    case State::Falling:
        enterFalling();
        break;
    case State::EndCheck:
        enterEndCheck();
        break;
    }
}

// 各状态的进入函数
void Board::enterWaitingForInput() {
    qDebug() << "进入状态：等待用户输入";
    // 在这个状态下，等待 BoardWidget 发射 userSelectedTwoPieces 信号
}

void Board::enterSwapping() {
    qDebug() << "进入状态：交换中";

    std::shared_ptr<Cube> cube_1 = GetCube(first_pos_);
    std::shared_ptr<Cube> cube_2 = GetCube(second_pos_);

    if(!cube_1 || !cube_2){
        qDebug() << "Swapping failed: One of the cubes is invalid.";
        transitionTo(State::WaitingForInput);
        return;
    }

    cube_1->SetPlaying(true);
    cube_2->SetPlaying(true);

    QPointF start_1 = GetRenderPos(first_pos_);
    QPointF end_1 = GetRenderPos(second_pos_);
    QPointF start_2 = GetRenderPos(second_pos_);
    QPointF end_2 = GetRenderPos(first_pos_);

    ani_manager_->AddAnimation(ani_factory_.MakeMoveAnimation(cube_1, start_1, end_1, cube_1->GetType(),true), AnimType::Swap);
    ani_manager_->AddAnimation(ani_factory_.MakeMoveAnimation(cube_2, start_2, end_2, cube_2->GetType(),true), AnimType::Swap);

    Swap(first_pos_, second_pos_);

    //添加这两个位置
    falling_cubes_.push_back(first_pos_);
    falling_cubes_.push_back(second_pos_);
    // 假设 Swap 是同步的，动画完成后会自动发射 swapAnimationsFinished 信号
}

void Board::enterCheckingMatch() {
    qDebug() << "进入状态：检查匹配";
    std::vector<std::vector<Vector2>> matches = CheckBoard();
    if (!matches.empty()) {
        qDebug() << "发现匹配，准备消除";
        emit matchFound();
    } else {
        qDebug() << "没有匹配，交换回去";
        emit noMatch();
    }
}

void Board::enterClearing() {
    qDebug() << "进入状态：消除中";
    std::vector<std::vector<Vector2>> matches = CheckBoard();
    SetRow4(matches);
    SetCol4(matches);

    matches = ProcessSpecialMatches(matches);

    int cleared = ClearCube(matches);
    qDebug() << "消除了" << cleared << "个方块";

    //添加动画的逻辑
    for(auto &cube_list:matches){
        for(Vector2& cube_pos:cube_list ){
            std::shared_ptr<Cube> cube = GetCube(cube_pos);
            QPointF pos = GetRenderPos(cube_pos);
            ani_manager_->AddAnimation(ani_factory_.MakeDestroyEffect(cube,pos),AnimType::Clear);
        }
    }

    // 如果有消除动画，确保在动画完成后发射 clearAnimationFinished 信号
    // 这里假设 ClearCube 是同步完成的，并立即发射信号
    // emit clearAnimationFinished();
}

void Board::enterFalling() {
    qDebug() << "进入状态：下落中";
    falling_cubes_.clear(); //进入之后先进行一次清理

    Fall();
    Fill();
    qDebug() << "下落并填充完成";

    // 如果有下落动画，确保在动画完成后发射 fallAnimationFinished 信号
    // 这里假设 Fall 和 Fill 是同步完成的，并立即发射信号
    // emit fallAnimationFinished();
}

void Board::enterEndCheck() {
    qDebug() << "进入状态：结束检查";
    std::vector<std::vector<Vector2>> new_matches = CheckBoard();
    if (!new_matches.empty()) {
        qDebug() << "发现新的匹配，继续消除";
        emit newMatchFound();
    } else {
        qDebug() << "没有新的匹配，等待用户输入";
        emit noNewMatch();
    }
}


std::vector<std::vector<Vector2>> Board::find_row_line4(const std::vector<std::vector<Vector2>> &matches) {
    std::vector<std::vector<Vector2>> row_line;

    for (const auto &group : matches) {
        if (group.size() >= 4) { // 至少包含 4 个方块
            // 检查是否所有元素都在同一行
            int row = group[0].GetRow();
            bool sameRow = std::all_of(group.begin(), group.end(), [row](const Vector2 &pos) {
                return pos.GetRow() == row;
            });

            if (!sameRow) continue; // 如果不在同一行，跳过

            // 提取所有的列号并排序
            std::vector<int> cols;
            for (const auto &pos : group) {
                cols.push_back(pos.GetColumn());
            }
            std::sort(cols.begin(), cols.end());

            // 检查是否存在至少 4 个连续的列号
            int consecutiveCount = 1;
            for (size_t i = 1; i < cols.size(); ++i) {
                if (cols[i] == cols[i - 1] + 1) {
                    ++consecutiveCount;
                    if (consecutiveCount >= 4) {
                        row_line.push_back(group);
                        break; // 找到符合条件的组合，跳出循环
                    }
                } else {
                    consecutiveCount = 1; // 重置计数器
                }
            }
        }
    }

    return row_line;
}


std::vector<std::vector<Vector2>> Board::find_col_line4(const std::vector<std::vector<Vector2>> &matches) {
    std::vector<std::vector<Vector2>> col_line;

    for (const auto &group : matches) {
        if (group.size() >= 4) { // 至少包含 4 个方块
            // 检查是否所有元素都在同一列
            int col = group[0].GetColumn();
            bool sameColumn = std::all_of(group.begin(), group.end(), [col](const Vector2 &pos) {
                return pos.GetColumn() == col;
            });

            if (!sameColumn) continue; // 如果不在同一列，跳过

            // 提取所有的行号并排序
            std::vector<int> rows;
            for (const auto &pos : group) {
                rows.push_back(pos.GetRow());
            }
            std::sort(rows.begin(), rows.end());

            // 检查是否存在至少 4 个连续的行号
            int consecutiveCount = 1;
            for (size_t i = 1; i < rows.size(); ++i) {
                if (rows[i] == rows[i - 1] + 1) {
                    ++consecutiveCount;
                    if (consecutiveCount >= 4) {
                        col_line.push_back(group);
                        break; // 找到符合条件的组合，跳出循环
                    }
                } else {
                    consecutiveCount = 1; // 重置计数器
                }
            }
        }
    }

    return col_line;
}

void Board::RemoveFromMatch(const Vector2 &pos, std::vector<std::vector<Vector2>> &matches) {
    for (auto it = matches.begin(); it != matches.end(); /* no increment here */) {
        auto &cube_list = *it;

        // 删除子向量中的 pos
        auto pos_it = std::remove(cube_list.begin(), cube_list.end(), pos);
        if (pos_it != cube_list.end()) {
            cube_list.erase(pos_it, cube_list.end()); // 真正删除元素
        }

        // 如果子向量为空，删除该子向量
        if (cube_list.empty()) {
            it = matches.erase(it); // 删除子向量，返回下一个有效迭代器
        } else {
            ++it; // 否则，继续检查下一个子向量
        }
    }
}


void Board::SetRow4(std::vector<std::vector<Vector2>> &matches){
    std::vector<std::vector<Vector2>> row4_list = find_row_line4(matches);
    // for(auto &cube_list:row4_list){
    //     for(Vector2& cube_pos:cube_list ){
    //         auto it = std::find(falling_cubes_.begin(), falling_cubes_.end(), cube_pos);
    //         if (it != falling_cubes_.end()) {
    //             // 找到了
    //             qDebug() << "row找到了";
    //             GetCube(*it)->SetSpecialType(Constants::kspecial_cube_column);
    //             RemoveFromMatch(*it,matches);
    //             //播放升级动画
    //         } else{
    //             GetCube(cube_list[0])->SetSpecialType(Constants::kspecial_cube_column);
    //             RemoveFromMatch(cube_list[0],matches);
    //         }
    //     }
    // }
    if(!row4_list.empty()){
        std::shared_ptr<Cube> cube = GetCube(row4_list[0][0]);
        cube->SetSpecialType(Constants::kspecial_cube_column);
        // RemoveFromMatch(cube->GetPos(),matches);
        // ani_manager_->AddAnimation(ani_factory_.MakeCubeAnimation(cube,GetRenderPos(cube->GetPos()),AnimationType::Col,true),AnimType::Extra);
    }

}

void Board::SetCol4(std::vector<std::vector<Vector2>> &matches){
    std::vector<std::vector<Vector2>> col4_list = find_col_line4(matches);
    // for(auto &cube_list:col4_list){
    //     for(Vector2& cube_pos:cube_list ){
    //         auto it = std::find(falling_cubes_.begin(), falling_cubes_.end(), cube_pos);
    //         if (it != falling_cubes_.end()) {
    //             // 找到了
    //             qDebug() << "col找到了";
    //             GetCube(*it)->SetSpecialType(Constants::kspecial_cube_row);
    //             RemoveFromMatch(*it,matches);
    //             //播放升级动画
    //         }
    //         else{
    //             GetCube(cube_list[0])->SetSpecialType(Constants::kspecial_cube_row);
    //             RemoveFromMatch(cube_list[0],matches);
    //         }
    //     }
    // }
    if(!col4_list.empty()){
        std::shared_ptr<Cube> cube = GetCube(col4_list[0][0]);
        cube->SetSpecialType(Constants::kspecial_cube_row);
        // RemoveFromMatch(cube->GetPos(),matches);
        // ani_manager_->AddAnimation(ani_factory_.MakeCubeAnimation(cube,GetRenderPos(cube->GetPos()),AnimationType::Row,true),AnimType::Extra);
    }

}


std::vector<std::vector<Vector2>> Board::ProcessSpecialMatches(const std::vector<std::vector<Vector2>> &matches) {
    std::set<std::pair<int, int>> processed; // 记录已处理的方块，避免重复
    std::vector<std::vector<Vector2>> result; // 最终的结果

    // 辅助函数：将方块加入结果并递归检查
    auto processCube = [&](const Vector2 &pos, auto &&processCubeRef) -> void {
        int row = pos.GetRow();
        int col = pos.GetColumn();

        // 如果已经处理过，直接返回
        if (!processed.insert({row, col}).second) return;

        // 获取当前方块
        if (!board_[row][col]) return;
        std::shared_ptr<Cube> cube = board_[row][col];

        // 添加当前方块到结果
        if (result.empty() || std::find(result.back().begin(), result.back().end(), pos) == result.back().end()) {
            result.back().push_back(pos);
        }

        // 根据特殊类型处理
        int special_type = cube->GetSpecialType();
        if (special_type == Constants::kspecial_cube_row) {
            // 清除整行
            for (int c = 0; c < GetWidth(); ++c) {
                processCubeRef(Vector2(row, c), processCubeRef);
            }
            // std::shared_ptr<MoveAnimation> ani = ani_factory_.MakeMoveAnimation(
            //     cube,
            //     ResourceManager::Instance().GetHLine(),
            //     GetRenderPos(cube->GetPos()),
            //     GetRenderPos(Vector2(cube->GetPos().GetRow(),-10)),
            //     cube->GetType(),
            //     false,
            //     1500,
            //     false);
            // ani_manager_->AddAnimation(ani,AnimType::Extra);

        } else if (special_type == Constants::kspecial_cube_column) {
            // 清除整列
            for (int r = 0; r < GetHeight(); ++r) {
                processCubeRef(Vector2(r, col), processCubeRef);
            }
        }
    };

    // 遍历初始 matches
    for (const auto &group : matches) {
        result.emplace_back(); // 开始一个新的组
        for (const auto &pos : group) {
            processCube(pos, processCube); // 处理当前方块
        }
    }

    return result;
}

















