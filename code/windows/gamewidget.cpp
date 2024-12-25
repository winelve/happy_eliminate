#include "gamewidget.h"
#include "ui_gamewidget.h"
#include "code/core/board.h"
#include "code/core/boardmanager.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QPainter>

#include <random>       // 用于 random_device, mt19937, uniform_int_distribution
#include <algorithm>    // 用于 shuffle
#include <utility>      // 用于 std::pair
#include <vector>       // 用于 std::vector

GameWidget::GameWidget(QWidget *parent)
    :QWidget(parent)
    , ui(new Ui::GameWidget)
{

    ui->setupUi(this);
    // 获取 DataResource 实例
    DataResource* resource = DataResource::instance();

    connect(resource, &DataResource::level_change_signal,
            this, &GameWidget::onLevelChanged);

    connect(resource, &DataResource::time_change_signal,
            this, &GameWidget::onTimeChanged);

    connect(resource, &DataResource::score_change_signal,
            this, &GameWidget::onScoreChanged);

    connect(resource, &DataResource::target_score_change_signal,
            this, &GameWidget::onTargetScoreChanged);

    connect(resource, &DataResource::step_change_signal,
            this, &GameWidget::onStepChanged);

    connect(this, &GameWidget::finished,
            ui->board_widget, &BoardWidget::InitData);

    emit finished(game_type_);
}
GameWidget::~GameWidget()
{
    delete ui;
}

void GameWidget::onLevelChanged(int level) {
    // 更新关卡显示
    // 假设有一个显示关卡的 label
    ui->show_level_label->setText(QString("关卡: %1").arg(level));
}

void GameWidget::onTimeChanged(int rest_time) {
    QString text;
    if(rest_time<Constants::k_Max_Time){
        text = QString("%1").arg(rest_time);
    }else {
        text = QString("∞");
    }
    ui->show_time_label->setText(text);
    int total_time = DataResource::instance()->total_time();  // 假设有这个方法

    // 计算进度并更新进度条
    int progress = (rest_time * 100) / total_time;  // 计算百分比
    ui->progressBar->setValue(progress);
}

void GameWidget::onScoreChanged(int score) {
    // 更新分数显示
    ui->show_socre_label->setText(QString("%1").arg(score));

}

void GameWidget::onTargetScoreChanged(int target_score) {
    // 更新目标分数显示
    ui->show_taget_label->setText(QString("%1").arg(target_score));

}

void GameWidget::onStepChanged(int rest_steps) {
    QString text;
    if(rest_steps<Constants::k_Max_Time){
        text = QString("%1").arg(rest_steps);
    }else {
        text = QString("∞");
    }

    // 更新剩余步数显示
    ui->show_step_label->setText(text);

}


void GameWidget::on_score_button_clicked()
{
    if(DataResource::instance()->score()<50){
        qDebug() << "分数不足以兑换";
        return ;
    }

    DataResource::instance()->set_score(DataResource::instance()->score()-50);
    DataResource::instance()->set_pace(DataResource::instance()->pace()*1.5);
    qDebug() << "分值倍率1.5";
}


void GameWidget::on_time_button_clicked()
{
    if(DataResource::instance()->score()<400){
        qDebug() << "分数不足以兑换";
        return ;
    }
    DataResource::instance()->set_score(DataResource::instance()->score()-500);
    DataResource::instance()->set_rest_time(DataResource::instance()->rest_time()+20);
    qDebug() << "延时20s";
}


void GameWidget::on_magic_button_clicked()
{
    int price = Constants::k_price_magic;
    if(DataResource::instance()->score()<price){
        qDebug() << "分数不足以兑换";
        return ;
    }
    DataResource::instance()->set_score(DataResource::instance()->score()-price);
    RandomMagic();
    qDebug() << "魔法猫咪";
}

void GameWidget::RandomMagic() {
    std::shared_ptr<Board> board = BoardManager::instance().GetCurrentBoard();
    int width = board->GetWidth();
    int height = board->GetHeight();

    // 用于生成随机数
    std::random_device rd;
    std::mt19937 gen(rd());

    // 生成1-3的随机数分布
    std::uniform_int_distribution<> type_dist(1, 3);

    // 存储所有可用位置
    std::vector<std::pair<int, int>> positions;
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            if(board->GetCube(i, j) != nullptr) {  // 确保位置有方块
                positions.push_back({i, j});
            }
        }
    }

    // 随机打乱位置
    std::shuffle(positions.begin(), positions.end(), gen);

    // 取前三个位置并设置随机值
    for(int i = 0; i < 3 && i < positions.size(); i++) {
        auto [row, col] = positions[i];
        int random_type = type_dist(gen);
        board->GetCube(row,col)->SetEliminate(random_type);
    }
}

