#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include "frameanimation.h"

#include <vector>
#include <QMutex>
#include <set>



// 定义动画类型枚举，重命名为 AnimType
enum class AnimType {
    Swap,
    Clear,
    Fall,
    Extra // 额外动画
};

class AnimationManager : public QObject
{
    Q_OBJECT
public:
    // 更新所有动画
    void UpdateAll(int delta_time_ms);

    // 绘制所有动画
    void DrawAll(QPainter &painter);

    // 添加带类型的动画
    void AddAnimation(const std::shared_ptr<FrameAnimation>& animation, AnimType type);

    // 添加额外的动画
    void AddExtraAnimation(const std::shared_ptr<FrameAnimation>& animation);

    // 移除指定动画
    void RemoveAnimation(const std::shared_ptr<FrameAnimation>& animation);

    // 移除所有动画
    void RemoveAll();

    // 获取单例实例
    static AnimationManager* GetInstance();

signals:
    // 当特定类型的所有动画完成时发射的信号
    void swapAnimationsFinished();
    void clearAnimationsFinished();
    void fallAnimationsFinished();
    void extraAnimationsFinished();

private:
    // 分类别存储动画
    std::vector<std::shared_ptr<FrameAnimation>> swap_animations_;
    std::vector<std::shared_ptr<FrameAnimation>> clear_animations_;
    std::vector<std::shared_ptr<FrameAnimation>> fall_animations_;
    std::vector<std::shared_ptr<FrameAnimation>> extra_animations_;

    // 检查并发射完成信号
    void CheckAndEmitFinished(const std::set<AnimType>& finishedTypes);
    // 互斥锁，确保线程安全
    mutable QMutex mutex_;

private: // 实现单例模式
    AnimationManager() = default;
    ~AnimationManager() = default;

    // 禁用拷贝和赋值
    AnimationManager(const AnimationManager&) = delete;
    AnimationManager& operator=(const AnimationManager&) = delete;
};


#endif // ANIMATIONMANAGER_H
