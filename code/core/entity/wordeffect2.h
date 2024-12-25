#ifndef WORDEFFECT2_H
#define WORDEFFECT2_H

#include "./entity.h"
#include "../Animation/resourcemanager.h"
#include "../Animation/rendermanager.h"

class WordEffect2:public Entity
{
public:
    WordEffect2(int state){
        SetLayer(1);
        AddAnimation("Word",ResourceManager::Instance().GetStateImg(state),2300,false);
        RenderManager::instance().AddEntityToGroup("Word",this);
        SetState("Word");
    }
    QSize GetRenderSize() override { return QSize(Constants::k_cell_size+500,Constants::k_cell_size+500); }
};

#endif // WORDEFFECT2_H
