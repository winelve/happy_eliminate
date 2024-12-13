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
}

GameWidget::~GameWidget()
{
    delete ui;
}
