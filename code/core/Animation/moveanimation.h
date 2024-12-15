#ifndef MOVEANIMATION_H
#define MOVEANIMATION_H

#include "frameanimation.h"
#include <memory>    // 引入智能指针
#include <QPainter> // 引入 QPainter

class MoveAnimation : public FrameAnimation
{
public:
    // 构造函数现在接受 std::shared_ptr<Cube>
    MoveAnimation(std::shared_ptr<Cube> cube);

    // 构造函数，指定起始位置、结束位置、帧列表、速度和是否循环移动
    MoveAnimation(
        std::shared_ptr<Cube> cube,
        QPointF start_pos,
        QPointF end_pos,
        const std::vector<QPixmap> &frame_list,
        float speed,
        bool loop_move = false,
        bool loop_frames = true
        );

    // 实现纯虚函数
    void update(int delta_time) override;
    void draw(QPainter &painter) override;

    // 设置结束位置
    void SetEndPosition(float x, float y);

    // 设置移动速度（像素每秒）
    void SetSpeed(float speed);

    // 设置是否循环移动
    void SetLoopMove(bool loop_move);
    // 设置动画结束后的延迟帧数
    void SetEndDelay(int delay_frames);
    void SetUseDelay(bool delay) { use_delay_ = delay; }

private:
    QPointF end_pos_;       // 终点位置
    QPointF direction_;     // 归一化的方向向量
    float speed_;           // 移动速度（像素/秒）
    bool loop_move_;        // 是否循环移动

    // 延迟相关成员变量
    int delay_frames_;      // 总的延迟帧数
    int current_delay_;     // 当前延迟帧数
    bool delay_active_;     // 是否处于延迟状态
    bool use_delay_ = false;

};

#endif // MOVEANIMATION_H
