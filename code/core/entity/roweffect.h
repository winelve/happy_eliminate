#ifndef ROWEFFECT_H
#define ROWEFFECT_H


#include "./entity.h"
#include "../Animation/resourcemanager.h"
#include "../Animation/rendermanager.h"


class RowEffect:public Entity
{
public:
    RowEffect(){
        std::vector<QPixmap> row_frame;
        row_frame.push_back(ResourceManager::Instance().GetHLine());
        AddAnimation("RowDestroy",row_frame,3000,false);
        RenderManager::instance().AddEntityToGroup("destroyEffect",this);
        SetState("RowDestroy");
    }
    QSize GetRenderSize() override { return QSize(Constants::k_cell_size+500,Constants::k_cell_size+500); }
};

#endif // ROWEFFECT_H
