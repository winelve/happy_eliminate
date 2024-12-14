#include "gamewidget.h"
#include "ui_gamewidget.h"

#include <QVBoxLayout>
#include <QPainter>

GameWidget::GameWidget(QWidget *parent)
    :QWidget(parent)
    , ui(new Ui::GameWidget)
    , board_widget_(this)
{
    ui->setupUi(this);
    // 创建布局并添加 BoardWidget
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(&board_widget_);
    setLayout(layout);
    resize(board_widget_.GetBoardSize() + QSize(50,50));

    // 设置定时器，定期调用 Update
    connect(&game_timer_, &QTimer::timeout, this, &GameWidget::onUpdate);
    game_timer_.start(16); // 大约60FPS
    elapsed_timer_.start();

    // AnimationManager* animation_manager_ = AnimationManager::GetInstance();
}

void GameWidget::onUpdate() {
    // 计算自上一次更新以来经过的时间（ms）
    int delta_time = elapsed_timer_.restart();

    board_widget_.onUpdate(delta_time);
    animation_manager_->UpdateAll(delta_time);
    update();
}


void GameWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    board_widget_.Draw(painter);
    animation_manager_->DrawAll(painter);
}




GameWidget::~GameWidget()
{
    delete ui;
}


