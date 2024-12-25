#ifndef MAGICEFFECT_H
#define MAGICEFFECT_H


#include "./entity.h"
#include "../Animation/resourcemanager.h"
#include "../Animation/rendermanager.h"


class MagicEffect:public Entity
{
public:
    MagicEffect(){
        AddAnimation("MagicDestroy",ResourceManager::Instance().GetMagicEffect(),Constants::k_frame_duration-10,false);
        RenderManager::instance().AddEntityToGroup("destroyEffect",this);
        SetState("MagicDestroy");
    }
    QSize GetRenderSize() override { return QSize(Constants::k_cell_size+200,Constants::k_cell_size+200); }
};

#endif // MAGICEFFECT_H
