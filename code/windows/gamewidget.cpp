#include "gamewidget.h"
#include "ui_gamewidget.h"
#include "code/core/board.h"
#include "code/core/boardmanager.h"
#include "code/core/utils/messagetips.h"
#include "code/audio/audioplayer.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QPainter>
#include <QFontDatabase>
#include <random>       // 用于 random_device, mt19937, uniform_int_distribution
#include <algorithm>    // 用于 shuffle
#include <utility>      // 用于 std::pair
#include <vector>       // 用于 std::vector

GameWidget::GameWidget(QWidget *parent)
    :QWidget(parent)
    , ui(new Ui::GameWidget)
{

    ui->setupUi(this);
    modifyFont();
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
    AudioPlayer::getInstance()->PlayBackgroundMusic("gamescenebgm.mp3");

    emit finished(game_type_);
}
GameWidget::~GameWidget()
{
    delete ui;
}

void GameWidget::CreateMessageBox(QString message){
    MessageTips *mTips=new MessageTips(message,this);
    mTips->show();
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
        text = QString("  %1").arg(rest_steps);
    }else {
        text = QString("  ∞");
    }

    // 更新剩余步数显示
    ui->show_step_label->setText(text);
}


void GameWidget::on_score_button_clicked()
{
    int price = Constants::k_price_pace;
    if(DataResource::instance()->score()<50){
        CreateMessageBox("分数不足以兑换,该道具需要"+QString::number(price)+"积分  (ノ—_—)ノ");
        return ;
    }

    DataResource::instance()->set_score(DataResource::instance()->score()-50);
    DataResource::instance()->set_pace(DataResource::instance()->pace()*1.5);

    CreateMessageBox("分值倍率1.5生效  Ciallo～(∠・ω< )⌒★");
}


void GameWidget::on_time_button_clicked()
{
    int price = Constants::k_price_time;
    if(DataResource::instance()->score()<400){
        CreateMessageBox("分数不足以兑换,该道具需要"+QString::number(price)+"积分  (ノ—_—)ノ");
        return ;
    }
    DataResource::instance()->set_score(DataResource::instance()->score()-500);
    DataResource::instance()->set_rest_time(DataResource::instance()->rest_time()+20);
    CreateMessageBox("延时道具生效,时间增加20s ≖‿≖✧");
}


void GameWidget::on_magic_button_clicked()
{
    int price = Constants::k_price_magic;
    if(DataResource::instance()->score()<price){
        CreateMessageBox("分数不足以兑换,该道具需要"+QString::number(price)+"积分  (ノ—_—)ノ ");
        return ;
    }
    DataResource::instance()->set_score(DataResource::instance()->score()-price);
    RandomMagic();
    CreateMessageBox("三个野生魔法猫咪出现了 (=ↀωↀ=)");
}

void GameWidget::on_reset_button_clicked()
{
    int price = Constants::k_price_switch;
    AudioPlayer::getInstance()->PlaySoundEffect("drop.mp3");
    if(DataResource::instance()->score()<price){
        CreateMessageBox("分数不足以兑换,该道具需要"+QString::number(price)+"积分  (ノ—_—)ノ");
        return ;
    }
    DataResource::instance()->set_score(DataResource::instance()->score()-price);
    ResetBoard();

    CreateMessageBox("棋盘已被随机打乱 ");
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
        if(random_type==3) board->GetCube(row,col)->SetType(-1);
    }
}

void GameWidget::modifyFont()
{
    // 设置字体
    int fontId = QFontDatabase::addApplicationFont(":/font/font.ttf");
    if (fontId == -1) {
        qWarning() << "没有找到字体！";
    } else {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont font(family);
        font.setPointSize(25);
        ui->show_name_label->setFont(font);
        font.setPointSize(15);
        ui->socre_label->setFont(font);
        ui->show_socre_label->setFont(font);
        ui->rest_time->setFont(font);
        ui->show_time_label->setFont(font);
        ui->target_label->setFont(font);
        ui->show_taget_label->setFont(font);
        font.setPointSize(20);
        ui->show_level_label->setFont(font);


    }
}

void GameWidget::ResetBoard() {
    std::shared_ptr<Board> board = BoardManager::instance().GetCurrentBoard();
    int width = board->GetWidth();
    int height = board->GetHeight();

    // 保存所有棋子的指针
    std::vector<std::shared_ptr<Cube>> cubes;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::shared_ptr<Cube> cube = board->GetCube(i, j);
            if (cube) {
                cubes.push_back(cube);
            }
        }
    }

    // 创建随机数生成器
    std::random_device rd;
    std::mt19937 gen(rd());

    // 打乱棋子位置
    bool validBoard = false;
    while (!validBoard) {
        std::shuffle(cubes.begin(), cubes.end(), gen);

        // 尝试放置棋子
        validBoard = true;
        int index = 0;

        for (int i = 0; i < height && validBoard; ++i) {
            for (int j = 0; j < width && validBoard; ++j) {
                // 检查当前位置是否会导致消除
                if (board->CausesMatch(i, j, cubes[index]->GetType())) {
                    validBoard = false;
                    break;
                }

                // 直接移动现有的Cube到新位置
                cubes[index]->PlayMotionAni("pos",Utils::GetRenderPos(cubes[index]->GetPos()),Utils::GetRenderPos(i,j),Constants::k_swap_duration,QEasingCurve(QEasingCurve::OutElastic));
                board->SetCube(i, j, cubes[index]);
                index++;
            }
        }
    }
}


void GameWidget::on_time_stoper_button_clicked()
{
    QTimer *timer = ui->board_widget->GetCountDownTimer();
    if(timer->isActive()){
        timer->stop();
        ui->board_widget->SetIsRender(false);
        ui->time_stoper_button->setIcon(QIcon(":/gui/gameWindow/9.png"));
    }else {
        timer->start();
        ui->board_widget->SetIsRender(true);
        ui->time_stoper_button->setIcon(QIcon(":/gui/gameWindow/8.png"));
    }
}
