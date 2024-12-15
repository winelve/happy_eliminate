#include "animationmanager.h"
#include <QDebug>


AnimationManager* AnimationManager::GetInstance(){
    static AnimationManager manager;
    return &manager;
}

void AnimationManager::UpdateAll(int delta_time_ms)
{
    QMutexLocker locker(&mutex_);

    std::set<AnimType> finishedTypes;

    // Lambda 函数用于更新动画并收集完成的类型
    auto updateAnimations = [&](std::vector<std::shared_ptr<FrameAnimation>>& animations, AnimType type) {
        for (auto it = animations.begin(); it != animations.end(); ) {
            // qDebug() << "Update:: type=" << static_cast<int>(type) << " size=" << animations.size();
            auto animation = *it;
            if (animation && animation->IsPlaying()) {
                animation->update(delta_time_ms);
                if (!animation->IsPlaying()) {
                    it = animations.erase(it);
                    finishedTypes.insert(type);
                    qDebug() << "Animation finished for type:" << static_cast<int>(type);
                    continue;
                }
            }
            ++it;
        }
    };

    // 并行更新所有动画类型
    updateAnimations(swap_animations_, AnimType::Swap);
    updateAnimations(clear_animations_, AnimType::Clear);
    updateAnimations(fall_animations_, AnimType::Fall);
    updateAnimations(extra_animations_, AnimType::Extra);

    // 在所有更新完成后，发射信号
    CheckAndEmitFinished(finishedTypes);
}

void AnimationManager::DrawAll(QPainter &painter)
{
    // 绘制所有动画
    for (const auto& animation : swap_animations_)
        if (animation) animation->draw(painter);
    for (const auto& animation : clear_animations_)
        if (animation) animation->draw(painter);
    for (const auto& animation : fall_animations_)
        if (animation) animation->draw(painter);
    for (const auto& animation : extra_animations_)
        if (animation) animation->draw(painter);
}

void AnimationManager::CheckAndEmitFinished(const std::set<AnimType>& finishedTypes)
{
    for (const auto& type : finishedTypes) {
        switch (type) {
        case AnimType::Swap:
            if (swap_animations_.empty()) {
                emit swapAnimationsFinished();
                qDebug() << "Emitted swapAnimationsFinished";
            }
            break;
        case AnimType::Clear:
            if (clear_animations_.empty()) {
                emit clearAnimationsFinished();
                qDebug() << "````````````````````````Emitted clearAnimationsFinished";
            }
            break;
        case AnimType::Fall:
            if (fall_animations_.empty()) {
                emit fallAnimationsFinished();
                qDebug() << "Emitted fallAnimationsFinished";
            }
            break;
        case AnimType::Extra:
            if (extra_animations_.empty()) {
                emit extraAnimationsFinished();
                qDebug() << "Emitted extraAnimationsFinished";
            }
            break;
        }
    }
}

void AnimationManager::AddAnimation(const std::shared_ptr<FrameAnimation>& animation, AnimType type)
{
    if (!animation) {
        qDebug() << "尝试添加一个空的动画。";
        return;
    }

    switch (type) {
    case AnimType::Swap:
        swap_animations_.push_back(animation);
        break;
    case AnimType::Clear:
        clear_animations_.push_back(animation);
        break;
    case AnimType::Fall:
        fall_animations_.push_back(animation);
        break;
    case AnimType::Extra:
        extra_animations_.push_back(animation);
        break;
    }
}

void AnimationManager::AddExtraAnimation(const std::shared_ptr<FrameAnimation>& animation)
{
    AddAnimation(animation, AnimType::Extra);
}

void AnimationManager::RemoveAnimation(const std::shared_ptr<FrameAnimation>& animation)
{
    auto removeFromList = [&](std::vector<std::shared_ptr<FrameAnimation>>& animations) {
        auto it = std::remove(animations.begin(), animations.end(), animation);
        if (it != animations.end()) {
            animations.erase(it, animations.end());
        }
    };

    removeFromList(swap_animations_);
    removeFromList(clear_animations_);
    removeFromList(fall_animations_);
    removeFromList(extra_animations_);
}

void AnimationManager::RemoveAll()
{
    swap_animations_.clear();
    clear_animations_.clear();
    fall_animations_.clear();
    extra_animations_.clear();
}
