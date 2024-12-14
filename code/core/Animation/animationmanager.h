#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include "frameanimation.h"

#include <vector>


class AnimationManager
{
public:

    void UpdateAll(int delta_time_ms);
    void DrawAll(QPainter &painter);
    void AddAnimation(const std::shared_ptr<FrameAnimation>& animation);
    void AddAnimationList(const std::vector<std::shared_ptr<FrameAnimation>>& animations);
    void RemoveAnimation(const std::shared_ptr<FrameAnimation>& animation);
    void RemoveAll();
    static AnimationManager *GetInstance();
private:
    std::vector<std::shared_ptr<FrameAnimation>> animation_list_;

private: //实现单例模式
    AnimationManager() = default;
    ~AnimationManager() = default;
};

#endif // ANIMATIONMANAGER_H
