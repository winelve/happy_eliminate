#include "posmanager.h"
#include "./utils/utils.h"
#include "./boardmanager.h"
#include "./board.h"
#include <QDebug>

PosManager::PosManager(QObject *parent)
    : QObject(parent), choosed_(false),available_(false) {}

PosManager* PosManager::instance() {
    static PosManager instance;  // 静态局部变量，确保只有一个实例
    return &instance;
}


void PosManager::HandleClick(const Vector2 &pos){

    if (!choosed_) {
        pos1_ = pos;
        choosed_ = true;
        if(BoardManager::instance().GetCurrentBoard()->GetCube(pos1_)->GetEliminate()==CubeState::Normal_Eliminate){
            BoardManager::instance().GetCurrentBoard()->GetCube(pos1_)->SetState("click");
        }


        //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>设置Cube的状态
        // GetCube(pos1_)->SetChoosed(true);
        qDebug() << "HandleMouseClick: First cube selected at ("
                 << pos1_.GetRow() << ", " << pos1_.GetColumn() << ").";
    }
    else {
        if(BoardManager::instance().GetCurrentBoard()->GetCube(pos1_)->GetEliminate()==CubeState::Normal_Eliminate){
            BoardManager::instance().GetCurrentBoard()->GetCube(pos1_)->SetState("normal");
        }
        // Second cube selection
        pos2_ = pos;
        qDebug() << "HandleMouseClick: Second cube selected at ("
                 << pos2_.GetRow() << ", " << pos2_.GetColumn() << ").";

        // Check if the second click is on the same cube as the first
        if (pos1_.GetRow() == pos2_.GetRow() && pos1_.GetColumn() == pos2_.GetColumn()) {
            qDebug() << "HandleMouseClick: Second click is the same as the first. Selection reset.";

            Reselect();
            return;
        }

        // Check if the two selected cubes are adjacent
        if (Utils::areAdjacent(pos1_, pos2_)) {
            qDebug() << "HandleMouseClick: Selected cubes are adjacent. Initiating swap.";

            //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
            // GetCube(pos1_)->SetChoosed(false);
            choosed_ = false;
            available_  =true;
            emit finish_choose_pos_singal();

        }
        else {
            qDebug() << "HandleMouseClick: Selected cubes are not adjacent. Selection reset.";
            Reselect();
        }
    }
}

void PosManager::Reselect(){
    choosed_ = false;
    available_ = false;
    //清除选中状态
    std::shared_ptr<Cube> first_cube = BoardManager::instance().GetCurrentBoard()->GetCube(pos1_);
    if(first_cube){
        if(BoardManager::instance().GetCurrentBoard()->GetCube(pos1_)->GetEliminate()==CubeState::Normal_Eliminate){
            BoardManager::instance().GetCurrentBoard()->GetCube(pos1_)->SetState("normal");
        }
    }
}



















