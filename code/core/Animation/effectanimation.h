// effectanimation.h
#ifndef EFFECTANIMATION_H
#define EFFECTANIMATION_H

#include "frameanimation.h"
#include "../constants.h"
#include <QPainter>
#include <memory>
#include <vector>
#include <QPointF>

class EffectAnimation : public FrameAnimation
{
public:
    // 构造函数，接受 Cube 对象和总时长
    EffectAnimation(std::shared_ptr<Cube> cube, int total_duration_ms);

    // 构造函数，接受 Cube 对象、位置和总时长
    EffectAnimation(std::shared_ptr<Cube> cube, QPointF pos, int total_duration_ms);

    // 构造函数，接受 Cube 对象、位置、帧列表和总时长
    EffectAnimation(std::shared_ptr<Cube> cube, QPointF pos, const std::vector<QPixmap> &frame_list, int total_duration_ms = 800);

    // 设置总时长（毫秒）
    void SetTotalDuration(int total_duration_ms);

    // 实现纯虚函数
    void update(int delta_time) override;
    void draw(QPainter &painter) override;

private:
    int total_duration_ms_;        // 总动画时长
    bool initialized_;             // 是否已初始化帧持续时间
    void InitializeFrameDuration();
};

#endif // EFFECTANIMATION_H
