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
#include "../entity/magiceffect.h"
#include "../entity/wordeffect.h"
#include "../entity/wordeffect2.h"
#include "../dataresource.h"
#include "code/audio/audioplayer.h"
#include "code/windows/mainwindow.h"
#include "code/database/database.h"
#include "code/windows/usermanager.h"
#include <QObject>
#include <QDebug>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QTimer>


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

    }

    void onEnter() override {
        QObject::connect(PosManager::instance(), &PosManager::finish_choose_pos_singal, this, &WaitingForInputState::HandleMouseSignal);

        // 重新连接
        qDebug() << "进入状态：等待用户输入";
        // 等待用户操作逻辑
        if(DataResource::instance()->rest_steps()==0){
            qDebug() << ">>>>>>StepEnd<<<<<<<";
            state_machine_->SwitchTo("StepEnd");
        }

    }
    void onUpdate() override {
        if(DataResource::instance()->game_over) {
            is_gameover_ = false;
            state_machine_->SwitchTo("GameOver");
            qDebug() << ">>>>>>GameOver<<<<<<<";
        }
    }
    void onExit() override {
        QObject::disconnect(PosManager::instance(), &PosManager::finish_choose_pos_singal, this, &WaitingForInputState::HandleMouseSignal);
    }

//这里写一个槽,用来接受信号-->已经完成点击事件的信号
public slots:
    void HandleMouseSignal(){
        qDebug() << "State1::finished";
        if(state_machine_){
            state_machine_->SwitchTo("Swapping");
        }
    }
    void HandleTimerOver(){
        is_gameover_ = true;
    }
private:
    bool is_gameover_ = false;

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
        //设置步数-1
        DataResource::instance()->set_rest_steps(DataResource::instance()->rest_steps()-1);
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
        connect(ani_group, &QParallelAnimationGroup::finished, this, &SwappingState::swap_ani_finished);
        ani_group->start(QAbstractAnimation::DeleteWhenStopped); // 使用 QAbstractAnimation

        //数据逻辑
        GameLogic::instance().Swap(board,pos1,pos2);
    }

public slots:
    //处理动画播放结束
    void transToChecking(){
        std::shared_ptr<Board> board = BoardManager::instance().GetCurrentBoard();
        Vector2 pos1 = PosManager::instance()->GetPos1();
        Vector2 pos2 = PosManager::instance()->GetPos2();
        //如果有魔法猫咪
        if(board->GetCube(pos1)->GetEliminate()==CubeState::Magic_Eliminate
            || board->GetCube(pos2)->GetEliminate()==CubeState::Magic_Eliminate)
        {
            qDebug() << ">>>>>>>>>>>>出现了  野生魔法猫咪 <<<<<<<<<<<<";
            state_machine_->SwitchTo("MagicClear");
        }
        else{
            qDebug() << "动画播放完了";
            state_machine_->SwitchTo("CheckingMatch");
        }

    }

signals:
    void swap_ani_finished();
};


class MagicClearState : public StateNode
{
    Q_OBJECT
public:
    explicit MagicClearState(QObject *parent = nullptr)
        : StateNode(parent)
    {
        connect(this, &MagicClearState::magic_ani_finished, this, &MagicClearState::transToFall);
    }

    void onEnter() override {
        std::shared_ptr<Board> board = BoardManager::instance().GetCurrentBoard();
        std::vector<std::vector<Vector2>> matches;
        Vector2 pos1 = PosManager::instance()->GetPos1();
        Vector2 pos2 = PosManager::instance()->GetPos2();

        int magic_num = 0;
        if(board->GetCube(pos1)->GetEliminate()==CubeState::Magic_Eliminate) magic_num++;
        if(board->GetCube(pos2)->GetEliminate()==CubeState::Magic_Eliminate) magic_num++;

        if(magic_num == 1){
            matches.resize(1);
            matches[0].push_back(GetMagicPos());

            GameLogic::instance().CheckMagic(GetElimiateType(),matches,board);
            GameLogic::instance().CheckSpecial(&matches);
            Utils::RemoveDuplicates(matches);
            MagicEffect* magic_effect = new MagicEffect();
            magic_effect->SetRenderPos(Utils::GetRenderPos(GetMagicPos()));
            // 使用 QTimer 延迟 0.5 秒后执行后续代码


        }

        else if(magic_num >= 2){
            qDebug() << "--------------------------Exit3<<<<<<<<<<<<<<<<<<<<<";

        }


        // 创建一个 QParallelAnimationGroup 来同时管理所有的透明度动画
        QParallelAnimationGroup* parallelGroup = new QParallelAnimationGroup(this);
        QParallelAnimationGroup* parallel_effect = new QParallelAnimationGroup(this);
        for (const auto& row : matches) {
            for (const auto& position : row) {
                // 假设你有一个方法根据 position 获取 Cube 对象
                Cube* cube = BoardManager::instance().GetCurrentBoard()->GetCube(position).get();
                // cube->SetState("");
                if (cube) {
                    // 创建透明度降低的动画
                    auto opacityAni = cube->CreatMotionAni("opacity", 1.0, 0.0, 200, QEasingCurve::OutQuad);
                    parallelGroup->addAnimation(opacityAni);
                    QTimer::singleShot(500, this, [=]() {
                      EliminateEffect *effect = new EliminateEffect(); effect->SetRenderPos(Utils::GetRenderPos(position));
                    });
                    if(cube->GetEliminate()==CubeState::Row_Eliminate){
                        RowEffect *row_effect1 = new RowEffect(); row_effect1->SetRenderPos(Utils::GetRenderPos(-10,10));
                        parallel_effect->addAnimation(row_effect1->CreatMotionAni("pos",Utils::GetRenderPos(cube->GetPos()),
                                                   Utils::GetRenderPos(cube->GetPos().GetRow(),-10),500));
                        RowEffect *row_effect2 = new RowEffect(); row_effect2->SetRenderPos(Utils::GetRenderPos(-10,10));
                        parallel_effect->addAnimation(row_effect2->CreatMotionAni("pos",Utils::GetRenderPos(cube->GetPos()),
                                                   Utils::GetRenderPos(cube->GetPos().GetRow(),board->GetWidth()+10),500));
                    }
                    if(cube->GetEliminate()==CubeState::Col_Eliminate){
                        ColEffect *col_effect1 = new ColEffect(); col_effect1->SetRenderPos(Utils::GetRenderPos(-10,10));
                        parallel_effect->addAnimation(col_effect1->CreatMotionAni("pos",Utils::GetRenderPos(cube->GetPos()),
                                                   Utils::GetRenderPos(-10,cube->GetPos().GetColumn()),500));
                        ColEffect *col_effect2 = new ColEffect(); col_effect2->SetRenderPos(Utils::GetRenderPos(-10,10));
                        parallel_effect->addAnimation(col_effect2->CreatMotionAni("pos",Utils::GetRenderPos(cube->GetPos()),
                                                   Utils::GetRenderPos(board->GetHeight()+10,cube->GetPos().GetColumn()),500));
                    }

                }
            }
        }
        // 连接 QParallelAnimationGroup 的 finished 信号，确保所有动画完成后触发回调
        connect(parallelGroup, &QParallelAnimationGroup::finished,this,&MagicClearState::magic_ani_finished);

        QTimer::singleShot(600, this, [=]() {
            parallelGroup->start();
        });
        QTimer::singleShot(500, this, [=]() {
            parallel_effect->start();
            AudioPlayer::getInstance()->PlaySoundEffect("swap_wrapwrap.mp3");
        });


        qDebug() << "--------------------------Exit1<<<<<<<<<<<<<<<<<<<<<";

        int clear_size = GameLogic::instance().ClearCube(board,matches);
        DataResource::instance()->add_score(clear_size);
        DataResource::instance()->set_pace(Constants::k_cube_pace);
    }

    void onExit() override {
        qDebug() << "Magic Exit";
    }

private:
    int GetElimiateType(){
        std::shared_ptr<Board> board = BoardManager::instance().GetCurrentBoard();
        Vector2 pos1 = PosManager::instance()->GetPos1();
        Vector2 pos2 = PosManager::instance()->GetPos2();
        int type = 0;
        if(board->GetCube(pos1)->GetEliminate()==CubeState::Magic_Eliminate){
            type = board->GetCube(pos2)->GetType();
        } else{
            type = board->GetCube(pos1)->GetType();
        }
        return type;
    }

    Vector2 GetMagicPos(){
        std::shared_ptr<Board> board = BoardManager::instance().GetCurrentBoard();
        Vector2 pos1 = PosManager::instance()->GetPos1();
        Vector2 pos2 = PosManager::instance()->GetPos2();
        if(board->GetCube(pos1)->GetEliminate()==CubeState::Magic_Eliminate){
            return pos1;
        }else{
            return pos2;
        }
    }

public slots:
    void transToFall(){
        qDebug() << "--------------------------Exit2<<<<<<<<<<<<<<<<<<<<<";
        state_machine_->SwitchTo("Falling");
        qDebug() << "--------------------------Exit2<<<<<<<<<<<<<<<<<<<<<";
    }
signals:
    void magic_ani_finished();
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
        qDebug() << "SSSSSSSSSSSSSSSize:" <<matches.size();

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
        connect(parallelGroup, &QParallelAnimationGroup::finished,this,&ClearingState::clear_ani_finished);
        parallelGroup->start();
        Utils::PlayEliminateMusic(DataResource::instance()->elimination_times());
        qDebug() << "DataResource::instance()->elimination_times()::::::::::::::::::::"<<DataResource::instance()->elimination_times();

        //逻辑消除
        int clear_size = GameLogic::instance().ClearCube(BoardManager::instance().GetCurrentBoard(),matches);
        DataResource::instance()->add_score(clear_size);
        DataResource::instance()->set_pace(Constants::k_cube_pace);
        qDebug() << "clear size: " << clear_size;
    }
private:
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
    }

    void onEnter() override {

        connect(&GameLogic::instance(), &GameLogic::fallEvent, this, &FallingState::onCubeFall);

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

    void onExit() override {
        // 先断开连接
        // disconnect(this, &FallingState::fall_ani_finished, this, &FallingState::transToEndCheck);
        disconnect(&GameLogic::instance(), &GameLogic::fallEvent, this, &FallingState::onCubeFall);
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
    explicit EndCheckState(QWidget *parent = nullptr)
        : StateNode(parent){}

    void onEnter() override {
        qDebug() << "进入---结束检查状态---";
        // 检查是否还有新的匹配
        auto newMatches = GameLogic::instance().CheckBoard(BoardManager::instance().GetCurrentBoard());
        //发现新的可消除序列
        if (!newMatches.empty()) {
            DataResource::instance()->elimination_times_add_one();
            state_machine_->SwitchTo("CheckingMatch");
        } else {
            //无可消除的序列
            if(DataResource::instance()->elimination_times()>=2){
                WordEffect *effect = new WordEffect(DataResource::instance()->elimination_times());
                effect->SetRenderPos(Utils::GetRenderPos(Constants::Word_Pos));
                QPropertyAnimation *ani = effect->CreatMotionAni("opacity",0,1,700,QEasingCurve::OutQuint);
                connect(ani, &QPropertyAnimation::finished, this, [this]() { transTo(); });
                ani->start();
                Utils::PlayLevelMusic(DataResource::instance()->elimination_times());
            }
            else {
                state_machine_->SwitchTo("WaitingForInput");
            }

            DataResource::instance()->reset_elimination_times();
        }
        qDebug() << ">>>>>>>>>>>>>>>>>>Times:" << DataResource::instance()->elimination_times();
    }
public slots:
    void transTo() {
        state_machine_->SwitchTo("WaitingForInput");
    }


};


//游戏步数结束状态 ---> 进入下一局
class StepEndState : public StateNode
{
    Q_OBJECT

public:
    explicit StepEndState(QObject *parent = nullptr)
        : StateNode(parent){}

    void onEnter() override {
        DataResource *data = DataResource::instance();
        if(data->score()>=data->target_score()){
            WordEffect2 *effect = new WordEffect2(1);
            effect->SetRenderPos(Utils::GetRenderPos(Constants::Word_Pos));
            QPropertyAnimation *ani = effect->CreatMotionAni("opacity",0,1,700,QEasingCurve::OutQuint);
            connect(ani, &QPropertyAnimation::finished, this, [this]() { InitNew(); });
            ani->start();
        }else {
            state_machine_->SwitchTo("GameOver");
        }

    }

public slots:
    void InitNew(){
        qDebug() << "进入下一关";
        DataResource::instance()->set_level(DataResource::instance()->level()+1);
        DataResource::instance()->set_target_score(DataResource::instance()->target_score() * DataResource::instance()->level());
        DataResource::instance()->set_score(0);
        DataResource::instance()->set_rest_steps(DataResource::instance()->total_steps() + 5);
        //初始化棋子
        // initBoard();
        std::shared_ptr<Board> board =  BoardManager::instance().GetCurrentBoard();
        board->InitRandomBoard(board->GetWidth(),board->GetHeight());
        state_machine_->SwitchTo("WaitingForInput");
    }

};

//游戏结束状态
class GameOverState : public StateNode
{
    Q_OBJECT
public:
    explicit GameOverState(QWidget *parent = nullptr)
        : StateNode(parent){}

    void onEnter() override {
        WordEffect2 *effect = new WordEffect2(0);
        effect->SetRenderPos(Utils::GetRenderPos(Constants::Word_Pos));
        QPropertyAnimation *ani = effect->CreatMotionAni("opacity",0,1,700,QEasingCurve::OutQuint);
        ani->start();
        qDebug() << "游戏结束,你的最终得分:" << DataResource::instance()->score();

        // 数据库连接相关
        DataBase *dataBase = new DataBase();
        dataBase->BuildDatabase();
        dataBase->updateUserScore(UserManager::getUsername(),DataResource::instance()->score(),UserManager::getUserkey());

        //执行结束逻辑
        //延时2.3~喵执行
        QTimer::singleShot(5000, this, [=]() {
            MainWindow *mw = new MainWindow();
            mw->show();
            // delay(150);
            //这个是BoardWidget
            QWidget *parentWidget = qobject_cast<QWidget *>(parent());
            //这个是GameWidget
            parentWidget->parentWidget()->parentWidget()->close();
        });

    }
    void onUpdate() override { }
};







#endif // BOARDSTATES_H
