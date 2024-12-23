#ifndef ELIMINATEEFFECT_H
#define ELIMINATEEFFECT_H

#include "./entity.h"
#include "../Animation/resourcemanager.h"
#include "../Animation/rendermanager.h"

class EliminateEffect:public Entity
{
public:
    EliminateEffect(){
        AddAnimation("destroy",ResourceManager::Instance().GetDestroyEffect(),
        Constants::k_frame_duration,false);

        RenderManager::instance().AddEntityToGroup("destroyEffect",this);

        SetState("destroy");
    }
    QSize GetRenderSize() override { return QSize(Constants::k_cell_size+15,Constants::k_cell_size+15); }
};

#endif // ELIMINATEEFFECT_H
