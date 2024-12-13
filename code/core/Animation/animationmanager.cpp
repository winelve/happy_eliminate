#include "animationmanager.h"
#include <QDebug>



AnimationManager* AnimationManager::GetInstance(){
    static AnimationManager manager;
    return &manager;
}

void AnimationManager::UpdateAll(int delta_time_ms)
{

    for (auto& animation : animation_list_)
    {
        if (animation->IsPlaying())
        {
            animation->update(delta_time_ms);
        }
    }
}

void AnimationManager::AddAnimation(const std::shared_ptr<FrameAnimation>& animation)
{
    if (animation)
    {
        animation_list_.push_back(animation);
    }
    else
    {
        qDebug() << "Attempted to add a null animation.";
    }
}

void AnimationManager::AddAnimationList(const std::vector<std::shared_ptr<FrameAnimation>>& animations)
{
    for (const auto& animation : animations)
    {
        if (animation)
        {
            animation_list_.push_back(animation);
        }
        else
        {
            qDebug() << "Attempted to add a null animation in the list." ;
        }
    }
}

void AnimationManager::RemoveAnimation(const std::shared_ptr<FrameAnimation>& animation)
{
    auto it = std::remove(animation_list_.begin(), animation_list_.end(), animation);
    if (it != animation_list_.end())
    {
        animation_list_.erase(it, animation_list_.end());
    }
    else
    {
        qDebug() << "Animation not found in the manager.";
    }
}

void AnimationManager::RemoveAll()
{
    animation_list_.clear();
}
