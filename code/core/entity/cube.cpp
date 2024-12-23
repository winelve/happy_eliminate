#include "cube.h"

#include "../Animation/resourcemanager.h"
#include "../utils/utils.h"

#include <QColor>
#include <QPen>
#include <QPainter>
#include <QDebug>

Cube::Cube(QWidget *parent)
    :Entity(parent), type_(0),eliminate_status_(CubeState::Normal_Eliminate)
{
    InitAniComponent();
}

Cube::Cube(int type,QWidget *parent)
    :Entity(parent), type_(type),eliminate_status_(CubeState::Normal_Eliminate)
{
    InitAniComponent();
}


Cube::Cube(int type, const Vector2& pos,QWidget *parent)
    :Entity(parent), type_(type), pos_(pos),eliminate_status_(CubeState::Normal_Eliminate)
{
    InitAniComponent();
}

void Cube::InitAniComponent(){
    int frame_duration = Constants::k_frame_duration;
    std::vector<QPixmap> normal_frame;
    normal_frame.push_back(ResourceManager::Instance().GetCube(type_));

    AddAnimation("normal",
                 normal_frame,
                 frame_duration,true);
    AddAnimation("click",
                 ResourceManager::Instance().GetAniResource(type_,AnimationType::Click),
                 frame_duration,true);
    AddAnimation("row",
                 ResourceManager::Instance().GetAniResource(type_,AnimationType::Row),
                 frame_duration,true);
    AddAnimation("col",
                 ResourceManager::Instance().GetAniResource(type_,AnimationType::Col),
                 frame_duration,true);
    AddAnimation("magic",
                 ResourceManager::Instance().GetMagic(),
                 frame_duration,true);

    SetState("normal");
    SetRenderPos(Utils::GetRenderPos(pos_));
    SetLayer(0);


}




