#ifndef COLEFFECT_H
#define COLEFFECT_H

#include "./entity.h"
#include "../Animation/resourcemanager.h"
#include "../Animation/rendermanager.h"

class ColEffect:public Entity
{
public:
    ColEffect(){
        std::vector<QPixmap> col_frame;
        col_frame.push_back(ResourceManager::Instance().GetVLine());
        AddAnimation("ColDestroy",col_frame,3000,false);
        RenderManager::instance().AddEntityToGroup("destroyEffect",this);
        SetState("ColDestroy");
    }
    QSize GetRenderSize() override { return QSize(Constants::k_cell_size+500,Constants::k_cell_size+500); }
};

#endif // COLEFFECT_H
