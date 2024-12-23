#include "animationComponent.h"

#include <QDebug>

AnimationComponent::AnimationComponent(QObject* parent)
    : QObject(parent), currentState(""), elapsedTime(0), currentFrameIndex(0), playing(false) {}

// 添加动画
void AnimationComponent::AddAnimation(const QString& state, const std::vector<QPixmap>& frames, int frameDuration, bool loop) {
    animations[state] = AnimationData{frames, frameDuration, loop};
}

// 设置当前动画状态
void AnimationComponent::SetState(const QString& state) {
    if (animations.contains(state) && state != currentState) {
        currentState = state;
        currentFrameIndex = 0;
        elapsedTime = 0;
        playing = true;
    }
}

// 更新动画
void AnimationComponent::update(int deltaTime) {
    if (!playing || currentState.isEmpty() || !animations.contains(currentState)) {
        return;
    }

    elapsedTime += deltaTime;
    AnimationData& anim = animations[currentState];

    // 切换到下一帧
    if (elapsedTime >= anim.frameDuration) {
        elapsedTime -= anim.frameDuration;
        currentFrameIndex++;

        // 如果到达最后一帧
        if (currentFrameIndex >= anim.frames.size()) {
            if (anim.loop) {
                currentFrameIndex = 0; // 循环播放
            } else {
                playing = false; // 停止播放
                currentFrameIndex = anim.frames.size() - 1; // 停留在最后一帧
            }
        }
    }
}

// 渲染当前动画帧
void AnimationComponent::render(QPainter& painter, const QPointF& position,const qreal opacity,const QSize &target_size) {

    if (currentState.isEmpty() || !animations.contains(currentState)) {
        return;
    }

    AnimationData& anim = animations[currentState];

    if (currentFrameIndex < anim.frames.size()) {
        // 设置目标尺寸，保留一些边

        // 缩放当前帧
        QPixmap scaledFrame = anim.frames[currentFrameIndex].scaled(
            target_size,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
            );

        // 计算居中位置
        float x = position.x() + (Constants::k_cell_size - scaledFrame.width()) / 2;
        float y = position.y() + (Constants::k_cell_size - scaledFrame.height()) / 2;
        painter.setOpacity(opacity);
        painter.drawPixmap(QPointF(x, y), scaledFrame);
        painter.setOpacity(1);
    }
}











