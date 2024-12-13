#include "gamewidget.h"
#include "ui_gamewidget.h"
#include <QVBoxLayout>

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
}

void GameWidget::onUpdate() {
    board_widget_.onUpdate();
}

GameWidget::~GameWidget()
{
    delete ui;
}


