#include "gamewidget.h"
#include "ui_gamewidget.h"
#include "code/core/Animation/rendermanager.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QPainter>

GameWidget::GameWidget(QWidget *parent)
    :QWidget(parent)
    , ui(new Ui::GameWidget)
    // , board_widget_(this)
{
    ui->setupUi(this);
    // 创建布局并添加 BoardWidget

    // QVBoxLayout *layout = new QVBoxLayout(this);
    // layout->addWidget(&board_widget_);
    // setLayout(layout);
    // resize(board_widget_.GetBoardSize() + QSize(50,50));
    // qDebug() << "---------------------------------Pos:" << ui->widget_2->mapToGlobal(QPoint(0,0));

    // 设置定时器，定期调用 Update
    connect(&game_timer_, &QTimer::timeout, this, &GameWidget::onUpdate);
    game_timer_.start(16); // 大约60FPS
    elapsed_timer_.start();

    ui->widget->setVisible(true);
    // ui->widget_2->setVisible(true);
    ui->widget_3->setVisible(true);

    // 强制布局更新
    updateGeometry();
    layout()->activate();

    qDebug() << "Main widget size:" << this->size();
    qDebug() << "Has layout:" << this->layout();
    qDebug() << "Widget 1 parent:" << ui->widget->parentWidget();
    qDebug() << "Widget 1 is visible:" << ui->widget->isVisible();


    qDebug() << "Widget 1 pos:" << ui->widget->pos();
    qDebug() << "Widget 1 global pos:" << ui->widget->mapToGlobal(QPoint(0,0));
    qDebug() << "Widget 1 geometry:" << ui->widget->geometry();

    // qDebug() << "Widget 2 pos:" << ui->widget_2->pos();
    // qDebug() << "Widget 2 global pos:" << ui->widget_2->mapToGlobal(QPoint(0,0));
    // qDebug() << "Widget 2 geometry:" << ui->widget_2->geometry();

    qDebug() << "Widget 3 pos:" << ui->widget_3->pos();
    qDebug() << "Widget 3 global pos:" << ui->widget_3->mapToGlobal(QPoint(0,0));
    qDebug() << "Widget 3 geometry:" << ui->widget_3->geometry();

    // Constants::k_board_padding = ui->widget_2->mapToGlobal(QPoint(0,0));

}

void GameWidget::onUpdate() {
    // 计算自上一次更新以来经过的时间（ms）
    int delta_time = elapsed_timer_.restart();
    board_widget_.Update(delta_time);
    RenderManager::instance().UpdateAll(delta_time);
    update(); //相当与render
}


void GameWidget::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);

    board_widget_.render(painter);

}


GameWidget::~GameWidget()
{
    delete ui;
}


