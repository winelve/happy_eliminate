#ifndef BOARDSTATES_H
#define BOARDSTATES_H

#include "StateNode.h"
#include "statemachine.h"
#include "../boardmanager.h"
#include "../gamelogic.h"
#include "../posmanager.h"
#include "../utils/utils.h"
#include "../entity/eliminateeffect.h"
#include "../entity/coleffect.h"
#include "../entity/roweffect.h"

#include <QObject>
#include <QDebug>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>


//
// 等待用户输入状态
//
class WaitingForInputState : public StateNode
{
    Q_OBJECT

public:
    explicit WaitingForInputState(QObject *parent = nullptr)
        : StateNode(parent)
    {
        connect(PosManager::instance(), &PosManager::finish_choose_pos_singal, this, &WaitingForInputState::HandleMouseSignal);
    }

    void onEnter() override {
        qDebug() << "进入状态：等待用户输入";
        // 等待用户操作逻辑
    }


//这里写一个槽,用来接受信号-->已经完成点击事件的信号
public slots:
    void HandleMouseSignal(){
        qDebug() << "State1::finished";
        if(state_machine_){
            state_machine_->SwitchTo("Swapping");
        }
    }
};

//
// 交换状态
//
class SwappingState : public StateNode
{
    Q_OBJECT

public:
    explicit SwappingState(QObject *parent = nullptr)
        : StateNode(parent)
    {
        //关联信号
        connect(this, &SwappingState::swap_ani_finished, this, &SwappingState::transToChecking);
    }

    void onEnter() override {
        qDebug() << "进入状态：交换中";
        if(!PosManager::instance()->Available()){
            qDebug() << "坐标未进行初始化";
            return ;
        }
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

        // 连接 finished 信号
        connect(ani_group, &QParallelAnimationGroup::finished, this, &SwappingState::swap_ani_finished,Qt::UniqueConnection);
        ani_group->start(QAbstractAnimation::DeleteWhenStopped); // 使用 QAbstractAnimation

        //数据逻辑
        GameLogic::instance().Swap(board,pos1,pos2);
    }

public slots:
    //处理动画播放结束
    void transToChecking(){
        if(state_machine_){
            state_machine_->SwitchTo("CheckingMatch");
        }
    }

signals:
    void swap_ani_finished();
};

//
// 检查匹配状态
//
class CheckingMatchState : public StateNode
{
    Q_OBJECT

public:
    explicit CheckingMatchState(QObject *parent = nullptr)
        : StateNode(parent)
    {
        connect(this, &CheckingMatchState::swap_back_ani_finished, this, &CheckingMatchState::transToWaiting);
    }

    void onEnter() override {
        qDebug() << "进入状态：检查匹配";

        // 检查匹配逻辑
        std::vector<std::vector<Vector2>> matches = GameLogic::instance().CheckBoard(BoardManager::instance().GetCurrentBoard());


        if (!matches.empty()) {
            state_machine_->SwitchTo("Clearing");
        } else {
            qDebug() << "没检测到能消除的";
            creatAni();
        }
    }

signals:
    void swap_back_ani_finished();
public slots:
    //处理动画播放结束-->回调函数
    void transToWaiting(){
        if(state_machine_){
            state_machine_->SwitchTo("WaitingForInput");
        }
    }
private:
    void creatAni();
};

//
// 消除状态
//
class ClearingState : public StateNode
{
    Q_OBJECT

public:
    explicit ClearingState(QObject *parent = nullptr)
        : StateNode(parent)
    {
        connect(this, &ClearingState::clear_ani_finished, this, &ClearingState::transToFall);
    }

    void onEnter() override {
        qDebug() << "进入状态：消除中";
        std::shared_ptr<Board> board = BoardManager::instance().GetCurrentBoard();
        std::vector<std::vector<Vector2>> matches = GameLogic::instance().CheckBoard(BoardManager::instance().GetCurrentBoard());

        //先标记特殊棋子
        GameLogic::instance().Find4(board,&matches);
        GameLogic::instance().Find5(board,&matches);
        //添加消除动画的逻辑

        Utils::PrintMatches(matches);
        qDebug() << "<<<<<<<<<<<<<<<<<<<<<In";
        GameLogic::instance().CheckSpecial(&matches);
        qDebug() << ">>>>>>>>>>>>>>>>>>>>>Out";
        Utils::PrintMatches(matches);


        // AddAnimation(matches);


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
                    parallelGroup->addAnimation(opacityAni);
                    EliminateEffect *effect = new EliminateEffect(); effect->SetRenderPos(Utils::GetRenderPos(position));

                    if(cube->GetEliminate()==CubeState::Row_Eliminate){
                        RowEffect *row_effect1 = new RowEffect();
                        row_effect1->PlayMotionAni("pos",Utils::GetRenderPos(cube->GetPos()),
                                                   Utils::GetRenderPos(cube->GetPos().GetRow(),-10),500);
                        RowEffect *row_effect2 = new RowEffect();
                        row_effect2->PlayMotionAni("pos",Utils::GetRenderPos(cube->GetPos()),
                                                   Utils::GetRenderPos(cube->GetPos().GetRow(),board->GetWidth()+10),500);
                    }
                    if(cube->GetEliminate()==CubeState::Col_Eliminate){
                        ColEffect *col_effect1 = new ColEffect();
                        col_effect1->PlayMotionAni("pos",Utils::GetRenderPos(cube->GetPos()),
                                                   Utils::GetRenderPos(-10,cube->GetPos().GetColumn()),500);
                        ColEffect *col_effect2 = new ColEffect();
                        col_effect2->PlayMotionAni("pos",Utils::GetRenderPos(cube->GetPos()),
                                                   Utils::GetRenderPos(board->GetHeight()+10,cube->GetPos().GetColumn()),500);
                    }

                }
            }
        }
        // 连接 QParallelAnimationGroup 的 finished 信号，确保所有动画完成后触发回调
        connect(parallelGroup, &QParallelAnimationGroup::finished,this,&ClearingState::clear_ani_finished, Qt::UniqueConnection);
        parallelGroup->start();
        //逻辑消除
        int clear_size = GameLogic::instance().ClearCube(BoardManager::instance().GetCurrentBoard(),matches);
        qDebug() << "clear size: " << clear_size;
    }
private:
    bool is_magic_clear_ = false;
    void AddAnimation(std::vector<std::vector<Vector2>> &matches);

public slots:
    void transToFall(){
        state_machine_->SwitchTo("Falling");
    }
signals:
    void clear_ani_finished();
};

//
// >>>>>>>>>>>>>>>>>下落状态<<<<<<<<<<<<<<<<
class FallingState : public StateNode
{
    Q_OBJECT
public:
    explicit FallingState(QObject *parent = nullptr)
        : StateNode(parent)
    {
        connect(this, &FallingState::fall_ani_finished, this, &FallingState::transToEndCheck);
        connect(&GameLogic::instance(), &GameLogic::fallEvent, this, &FallingState::onCubeFall);
    }

    void onEnter() override {
        qDebug() << "进入状态：下落中";
        std::shared_ptr<Board> board = BoardManager::instance().GetCurrentBoard();
        ani_group = new QParallelAnimationGroup(this);
        connect(ani_group, &QParallelAnimationGroup::finished,this,&FallingState::fall_ani_finished);

        //1.fall
        GameLogic::instance().Fall(board);
        //2.fill
        GameLogic::instance().Fill(board);

        //3.这时候动画已经添加完成了
        //4.播放动画
        if(ani_group) {
            ani_group->start(QAbstractAnimation::DeleteWhenStopped); // 使用 QAbstractAnimation
        }

    }
private:
    QParallelAnimationGroup* ani_group;

public slots:
    //接收动画生成事件的槽函数
    void onCubeFall(int fromRow, int fromCol, int toRow, int toCol,std::shared_ptr<Cube> cube) {

        std::shared_ptr<Board> board = BoardManager::instance().GetCurrentBoard();
        // 获取起始和结束位置
        float use_time = 100;
        int time = (toRow - fromRow) * use_time;
        QVariant startPos1 = Utils::GetRenderPos(fromRow,fromCol);
        QVariant endPos1 = Utils::GetRenderPos(toRow,toCol);
        if(ani_group){
            ani_group->addAnimation(cube->CreatMotionAni("pos",startPos1,endPos1,time,QEasingCurve::InQuad));
            qDebug() << "Cube 下落: 从 (" << fromRow << "," << fromCol << ") 到 (" << toRow << "," << toCol << ")";
        }

    }
    //对应自身切换状态的槽函数
    void transToEndCheck(){
        state_machine_->SwitchTo("EndCheck");
    }
signals:
    void fall_ani_finished();
};


//
// >>>>>>>>>>>>>>>>>结束检查状态<<<<<<<<<<<<<<<<
class EndCheckState : public StateNode
{
    Q_OBJECT

public:
    explicit EndCheckState(QObject *parent = nullptr)
        : StateNode(parent){}

    void onEnter() override {
        qDebug() << "进入---结束检查状态---";
        // 检查是否还有新的匹配
        auto newMatches = GameLogic::instance().CheckBoard(BoardManager::instance().GetCurrentBoard());

        if (!newMatches.empty()) {
            //发现新的可消除序列
            state_machine_->SwitchTo("CheckingMatch");
        } else {
            //未发现新的消除序列
            state_machine_->SwitchTo("WaitingForInput");
        }
    }

};

#endif // BOARDSTATES_H
