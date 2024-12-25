#ifndef WORDEFFECT_H
#define WORDEFFECT_H

#include "./entity.h"
#include "../Animation/resourcemanager.h"
#include "../Animation/rendermanager.h"

class WordEffect:public Entity
{
public:
    WordEffect(int times){
        std::vector<QPixmap> word_frame;
        word_frame.push_back(ResourceManager::Instance().GetWordEffect(times));
        SetLayer(1);
        AddAnimation("Word",word_frame,1000,false);
        RenderManager::instance().AddEntityToGroup("Word",this);
        SetState("Word");
    }
    QSize GetRenderSize() override { return QSize(Constants::k_cell_size+300,Constants::k_cell_size+300); }
};

#endif // WORDEFFECT_H
