#include "boardstates.h"

void CheckingMatchState::creatAni(){
    //未检测到匹配 //-->换回去
    //调用交换方法
    std::shared_ptr<Board> board = BoardManager::instance().GetCurrentBoard();
    Vector2 pos1 = PosManager::instance()->GetPos1();
    Vector2 pos2 = PosManager::instance()->GetPos2();
    // 获取起始和结束位置
    QVariant startPos1 = Utils::GetRenderPos(pos1);
    QVariant endPos1 = Utils::GetRenderPos(pos2);
    QVariant startPos2 = Utils::GetRenderPos(pos2);
    QVariant endPos2 = Utils::GetRenderPos(pos1);
    // 添加交换动画
    QParallelAnimationGroup* ani_group = new QParallelAnimationGroup(this); // 设置父对象为 this
    //ani_1
    ani_group->addAnimation(board->GetCube(pos1)->CreatMotionAni(
        "pos",startPos1,endPos1,Constants::k_swap_duration+300,QEasingCurve::OutElastic));
    //ani_2
    ani_group->addAnimation(board->GetCube(pos2)->CreatMotionAni(
        "pos",startPos2,endPos2,Constants::k_swap_duration+300,QEasingCurve::OutElastic));
    connect(ani_group, &QParallelAnimationGroup::finished, this, &CheckingMatchState::swap_back_ani_finished);
    ani_group->start(QAbstractAnimation::DeleteWhenStopped); // 使用 QAbstractAnimation

    AudioPlayer::getInstance()->PlaySoundEffect("drop.mp3");
    //数据逻辑
    GameLogic::instance().Swap(board,pos1,pos2);
}


void ClearingState::AddAnimation(std::vector<std::vector<Vector2>> &matches){
    // 创建一个 QParallelAnimationGroup 来同时管理所有的透明度动画
    QParallelAnimationGroup* parallelGroup = new QParallelAnimationGroup(this);
    for (const auto& row : matches) {
        for (const auto& position : row) {
            // 假设你有一个方法根据 position 获取 Cube 对象
            Cube* cube = BoardManager::instance().GetCurrentBoard()->GetCube(position).get();
            // cube->SetState("");
            if (cube) {
                // 创建透明度降低的动画
                auto opacityAni = cube->CreatMotionAni("opacity", 1.0, 0.0, 200, QEasingCurve::OutQuad);
                // 将当前的透明度动画添加到 QParallelAnimationGroup 中
                parallelGroup->addAnimation(opacityAni);

                EliminateEffect *effect = new EliminateEffect(); effect->SetRenderPos(Utils::GetRenderPos(position));
                qDebug() << "add";

            }
        }
    }
    // 连接 QParallelAnimationGroup 的 finished 信号，确保所有动画完成后触发回调
    connect(parallelGroup, &QParallelAnimationGroup::finished,this,&ClearingState::clear_ani_finished, Qt::UniqueConnection);
    parallelGroup->start();
}
