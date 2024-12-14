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
        if(animation){ //先判断空不空
            if (animation->IsPlaying()){
                qDebug() << "!!!!!!!!!!!!!!";
                animation->update(delta_time_ms);
            } else {
                qDebug() << "End";
                //不播放就删除
                RemoveAnimation(animation);
                qDebug() << "End After";
            }
        }
    }
}

void AnimationManager::DrawAll(QPainter &painter)
{
    // 遍历所有动画并调用它们的 draw 函数
    for (const auto& animation : animation_list_)
    {
        if (animation) { // 确保动画指针有效
            animation->draw(painter);
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
