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
        "pos",startPos1,endPos1));
    //ani_2
    ani_group->addAnimation(board->GetCube(pos2)->CreatMotionAni(
        "pos",startPos2,endPos2));
    connect(ani_group, &QParallelAnimationGroup::finished, this, &CheckingMatchState::swap_back_ani_finished);
    ani_group->start(QAbstractAnimation::DeleteWhenStopped); // 使用 QAbstractAnimation

    //数据逻辑
    GameLogic::instance().Swap(board,pos1,pos2);
}
