#include "boardwidget.h"

#include "./utils/constants.h"
#include "./utils/vector2.h"
#include "boardmanager.h"
#include "posmanager.h"
#include "board.h"

#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

BoardWidget::BoardWidget(QWidget *parent)
    : QWidget(parent)
{
    InitData();
    connect(&game_timer_, &QTimer::timeout, this, &BoardWidget::Update);
    game_timer_.start(16); // 大约60FPS
    elapsed_timer_.start();

    Constants::k_board_padding = QPoint(30, 30);
    qDebug() << "---------------------------------Pos:" << this->mapToParent(QPoint(0,0));
    //给参数赋值
    cell_size_ = Constants::k_cell_size;
    padding_ = Constants::k_board_padding;
    width_ = Constants::board_width;
    height_ = Constants::board_height;
    setFixedSize(GetBoardSize() + QSize(2 * padding_.x(), 2 * padding_.y()));

    //重要
    BoardManager::instance().AddBoard("default",std::make_shared<Board>(width_,height_,this));
    InitStateMachine();
    StartCounter(); // 启用计时器
}

void BoardWidget::DrawBK(int start_x, int start_y, int board_width, int board_height, QPainter &painter)
{
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制背景（可选）
    painter.setBrush(QBrush(QColor(31, 47, 70)));
    painter.drawRect(start_x, start_y, board_width, board_height); // 添加 10px 的圆角
}

void BoardWidget::DrawSelect(QPainter &painter){
    if(!PosManager::instance()->IsChoosed()){
        return ;
    }
    QPointF select_pos = Utils::GetRenderPos(PosManager::instance()->GetPos1());

    int x = select_pos.x();
    int y = select_pos.y();
    int size = Constants::k_cell_size;
    int cornerLength = size / 4;

    // 添加内发光效果
    QColor glowColor(255, 100, 100, 50); // 淡红色半透明
    painter.setPen(Qt::NoPen);
    painter.setBrush(glowColor);
    painter.drawRect(x, y, size, size);

    // 设置抗锯齿和平滑效果
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // 绘制渐变边角
    QLinearGradient gradient;
    gradient.setColorAt(0, Qt::yellow); // 亮红色
    gradient.setColorAt(1, QColor(255, 253, 208)  ); // 淡红色

    QPen cornerPen(QPen(QBrush(gradient), 3, Qt::SolidLine, Qt::RoundCap));
    painter.setPen(cornerPen);

    // 左上角
    gradient.setStart(x, y);
    gradient.setFinalStop(x + cornerLength, y);
    cornerPen.setBrush(gradient);
    painter.setPen(cornerPen);
    painter.drawLine(x, y, x + cornerLength, y);

    gradient.setStart(x, y);
    gradient.setFinalStop(x, y + cornerLength);
    cornerPen.setBrush(gradient);
    painter.setPen(cornerPen);
    painter.drawLine(x, y, x, y + cornerLength);

    // 右上角
    gradient.setStart(x + size - cornerLength, y);
    gradient.setFinalStop(x + size, y);
    cornerPen.setBrush(gradient);
    painter.setPen(cornerPen);
    painter.drawLine(x + size - cornerLength, y, x + size, y);

    gradient.setStart(x + size, y);
    gradient.setFinalStop(x + size, y + cornerLength);
    cornerPen.setBrush(gradient);
    painter.setPen(cornerPen);
    painter.drawLine(x + size, y, x + size, y + cornerLength);

    // 左下角
    gradient.setStart(x, y + size - cornerLength);
    gradient.setFinalStop(x, y + size);
    cornerPen.setBrush(gradient);
    painter.setPen(cornerPen);
    painter.drawLine(x, y + size - cornerLength, x, y + size);

    gradient.setStart(x, y + size);
    gradient.setFinalStop(x + cornerLength, y + size);
    cornerPen.setBrush(gradient);
    painter.setPen(cornerPen);
    painter.drawLine(x, y + size, x + cornerLength, y + size);

    // 右下角
    gradient.setStart(x + size - cornerLength, y + size);
    gradient.setFinalStop(x + size, y + size);
    cornerPen.setBrush(gradient);
    painter.setPen(cornerPen);
    painter.drawLine(x + size - cornerLength, y + size, x + size, y + size);

    gradient.setStart(x + size, y + size - cornerLength);
    gradient.setFinalStop(x + size, y + size);
    cornerPen.setBrush(gradient);
    painter.setPen(cornerPen);
    painter.drawLine(x + size, y + size - cornerLength, x + size, y + size);
}
void BoardWidget::render(QPainter &painter)
{
    std::shared_ptr<Board> board = BoardManager::instance().GetCurrentBoard();
    // 获取棋盘的尺寸
    int rows = board->GetHeight();
    int cols = board->GetWidth();

    // 计算棋盘的总宽度和高度
    int board_width = cols * cell_size_;
    int board_height = rows * cell_size_;

    // 设置棋盘的起始位置（根据需要调整）
    int start_x = Constants::k_board_padding.x();
    int start_y = Constants::k_board_padding.y();

    DrawBK(start_x, start_y, board_width, board_height, painter);

    // 绘制垂直线条
    painter.setPen(QPen(Qt::black, 2));
    for(int col = 0; col <= cols; ++col){
        int x = start_x + col * cell_size_;
        painter.drawLine(x, start_y, x, start_y + board_height);
    }

    // 绘制水平线条
    for(int row = 0; row <= rows; ++row){
        int y = start_y + row * cell_size_;
        painter.drawLine(start_x, y, start_x + board_width, y);
    }
    DrawSelect(painter);


    int w = board->GetWidth();
    int h = board->GetHeight();

    RenderManager::instance().RenderAll(painter);
    for(int i=0;i<w;i++){
        for(int j = 0;j<h;j++){
            board->GetCube(i,j)->render(painter);

        }
    }
    RenderManager::instance().RenderAll(painter,1);

}

void BoardWidget::Update(){
    int deltatime = elapsed_timer_.restart();
    std::shared_ptr<Board> board = BoardManager::instance().GetCurrentBoard();

    int w = board->GetWidth();
    int h = board->GetHeight();

    for(int i=0;i<w;i++){
        for(int j = 0;j<h;j++){
            board->GetCube(i,j)->update(deltatime);
        }
    }
    RenderManager::instance().UpdateAll(deltatime);
    // state_machine_.update();
    if(state_machine_.GetCurrentState()=="WaitingForInput"){
        state_machine_.GetCurrentNode()->onUpdate();
    }

    update();

}

// BoardWidget.cpp

void BoardWidget::mousePressEvent(QMouseEvent *ev)
{
    if(state_machine_.GetCurrentState() != "WaitingForInput") return ;
    // 处理左键点击事件
    if (ev->button() == Qt::LeftButton) {
        // // 获取鼠标点击的像素坐标
        // int x = ev->pos().x();
        // int y = ev->pos().y();

        // 获取鼠标的全局坐标
        QPointF globalPos = ev->globalPosition();
        // 将全局坐标转换为 BoardWidget 的局部坐标
        QPointF localPos = this->mapFromGlobal(globalPos);

        qDebug() << "Global Position:" << globalPos;
        qDebug() << "Local Position:" << localPos;


        Vector2 board_pos;
        if (!PixelToBoard(localPos.x(), localPos.y(), board_pos)) {
            qDebug() << "Clicked outside the board.";
            return;
        }

        PosManager::instance()->HandleClick(board_pos);
    }
    // 处理右键点击事件
    else if (ev->button() == Qt::RightButton) {
        PosManager::instance()->Reselect();
        qDebug() << "重置选择";
    }

    // 调用基类的事件处理
    // QWidget::mousePressEvent(ev);
}

bool BoardWidget::PixelToBoard(int x, int y, Vector2 &pos)
{
    qDebug() << "Mouse_x:" << x << "\t" << "Mouse_y:" << y;
    // 调整坐标，去除边距
    x -= padding_.x();
    y -= padding_.y();
    qDebug() << "Draw_x:" << x << "\t" << "Draw_y:" << y;

    // 检查点击是否在棋盘区域内
    if (x < 0 || y < 0) return false;

    int row = y / cell_size_;
    int col = x / cell_size_;

    pos.SetColumn(col);
    pos.SetRow(row);

    // 检查行列是否超出棋盘范围
    if (row < 0 || row >= height_ || col < 0 || col >= width_) {
        return false;
    }

    return true;
}


void BoardWidget::StartCounter(){
    disconnect(&count_down_timer_, &QTimer::timeout, this, nullptr);
    connect(&count_down_timer_, &QTimer::timeout, this, [this]() {  // 使用 [this] 而不是 [&]
        DataResource* resource = DataResource::instance();
        // 获取剩余时间
        int rest_time = resource->rest_time();

        if (rest_time <= 0) {
            // 如果时间已经到了，停止定时器
            count_down_timer_.stop();
            resource->timeout();  // 发出 timeout 信号
            DataResource::instance()->game_over = true;
            return;
        }
        // 更新剩余时间
        resource->set_rest_time(rest_time - 1);

    });
    count_down_timer_.start(1000);
}



void BoardWidget::InitStateMachine(){
    state_machine_.RegisterState("WaitingForInput",std::make_shared<WaitingForInputState>(this));
    state_machine_.RegisterState("Swapping",std::make_shared<SwappingState>(this));
    state_machine_.RegisterState("CheckingMatch",std::make_shared<CheckingMatchState>(this));
    state_machine_.RegisterState("MagicClear",std::make_shared<MagicClearState>(this));
    state_machine_.RegisterState("Clearing",std::make_shared<ClearingState>(this));
    state_machine_.RegisterState("Falling",std::make_shared<FallingState>(this));
    state_machine_.RegisterState("EndCheck",std::make_shared<EndCheckState>(this));
    state_machine_.RegisterState("StepEnd",std::make_shared<StepEndState>(this));
    state_machine_.RegisterState("GameOver",std::make_shared<GameOverState>(this));
    state_machine_.SwitchTo("WaitingForInput"); //设置初始状态
}



void BoardWidget::InitData(){
    int game_difficulty = ModeManager::getinstance()->getDifficulty();
    int board_size = ModeManager::getinstance()->getCheckerboard();
    int game_mode = ModeManager::getinstance()->getPlay();

    qDebug() << "game_difficulty:" << game_difficulty;
    qDebug() << "board_size:" << board_size;
    qDebug() << "game_mode:" << game_mode;

    SetDifficulty(game_difficulty);
    SetBoardSize(board_size);
    SetGameMode(game_mode);


    // if(choice==1){
    //     int time = 1000;
    //     int steps = 10;
    //     DataResource::instance()->set_level(1);
    //     DataResource::instance()->set_rest_steps(steps);
    //     DataResource::instance()->set_total_steps(steps);
    //     DataResource::instance()->set_score(0);
    //     DataResource::instance()->set_target_score(300);
    //     DataResource::instance()->set_rest_time(time);
    //     DataResource::instance()->set_total_time(time);
    // }
}




void BoardWidget::SetDifficulty(int choice) {

    switch (choice) {
    case 1: // 简单
        Constants::ktype_size = 4;
        break;
    case 2: // 中等
        Constants::ktype_size = 5;
        break;
    case 3: // 困难
        Constants::ktype_size = 6;
        break;
    default:
        Constants::ktype_size = 6;
        break;
    }
}

void BoardWidget::SetBoardSize(int choice) {
    switch (choice) {
    case 1: // 小
        Constants::board_width = 8;
        Constants::board_height = 8;
        Constants::k_cell_size = 64;
        break;
    case 2: // 中
        Constants::board_width = 10;
        Constants::board_height = 10;
        Constants::k_cell_size = 50;
        break;
    case 3: // 大
        Constants::board_width = 12;
        Constants::board_height = 12;
        Constants::k_cell_size = 42;
        break;
    default:
        Constants::board_width = 8;
        Constants::board_height = 8;
        Constants::k_cell_size = 64;
        break;
    }
    Constants::k_cube_size.setWidth(Constants::k_cell_size - 5);
    Constants::k_cube_size.setWidth(Constants::k_cell_size - 5);

}

void BoardWidget::SetGameMode(int choice) {

    switch (choice) {
    case 1: { // 闯关
        int time = 1000000;
        int steps = 10;
        DataResource::instance()->set_level(1);
        DataResource::instance()->set_rest_steps(steps);
        DataResource::instance()->set_total_steps(steps);
        DataResource::instance()->set_score(0);
        DataResource::instance()->set_target_score(1500);
        DataResource::instance()->set_rest_time(time);
        DataResource::instance()->set_total_time(time);
        break;
    }
    case 2: {
        // 计时
        int time = 120;
        int steps = 100000;
        DataResource::instance()->set_level(-1);
        DataResource::instance()->set_rest_steps(steps);
        DataResource::instance()->set_total_steps(steps);
        DataResource::instance()->set_score(0);
        DataResource::instance()->set_target_score(0);
        DataResource::instance()->set_rest_time(time);
        DataResource::instance()->set_total_time(time);
        break;
    }
    }
}




