#ifndef MOVEANIMATION_H
#define MOVEANIMATION_H

#include "frameanimation.h"

class MoveAnimation : public FrameAnimation
{
public:
    // 默认构造函数
    MoveAnimation();

    // 构造函数，指定起始位置、结束位置、帧列表、速度和是否循环移动
    MoveAnimation(QPointF start_pos, QPointF end_pos, const std::vector<QPixmap> &frame_list, float speed, bool loop_move = false, bool loop_frames = true);

    // 实现虚函数
    void update(int delta_time) override;
    void draw(QPainter &painter) override;

    // 设置结束位置
    void SetEndPosition(float x, float y);

    // 设置移动速度（像素每秒）
    void SetSpeed(float speed);

    // 设置是否循环移动
    void SetLoopMove(bool loop_move);

private:
    QPointF end_pos_;       // 终点位置
    QPointF direction_;     // 归一化的方向向量
    float speed_;           // 移动速度（像素/秒）
    bool loop_move_;        // 是否循环移动
};

#endif // MOVEANIMATION_H
