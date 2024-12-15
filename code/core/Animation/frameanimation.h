// frameanimation.h
#ifndef FRAMEANIMATION_H
#define FRAMEANIMATION_H

#include <vector>
#include <QRect>
#include <QPixmap>
#include <QPainter> // 添加 QPainter 头文件
#include <memory>   // 引入智能指针
#include "../cube.h"

class FrameAnimation
{
public:
    // 构造函数现在接受 std::shared_ptr<Cube>
    FrameAnimation(std::shared_ptr<Cube> cube);
    FrameAnimation(std::shared_ptr<Cube> cube, QPointF pos);
    FrameAnimation(std::shared_ptr<Cube> cube, QPointF pos, std::vector<QPixmap> frame_list);

    void AddFrame(const QPixmap &img);
    void AddFrameList(const std::vector<QPixmap> &frame_list);
    void SetFrameList(const std::vector<QPixmap> &frame_list);

    const QPixmap& GetCurrentFrame() const; // 获取当前帧
    void SetLooping(bool loop);              // 设置是否循环
    void SetFrameDuration(int ms);           // 设置每帧持续时间（毫秒）
    void SetPosition(float x, float y);      // 设置动画位置
    bool IsPlaying() const { return is_playing_; } // 获取播放状态
    void SetPlaying(bool playing) { is_playing_ = playing; } // 设置播放状态

    virtual void update(int delta_time) = 0; // ms
    virtual void draw(QPainter &painter) = 0;

protected:
    std::shared_ptr<Cube> cube_;    // 使用智能指针管理 Cube 对象
    QPointF start_pos_;             // 动画的起始位置

    bool is_loop_;                  // 是否循环
    bool is_playing_;               // 是否正在播放
    int current_frame_idx_;         // 当前帧的索引
    int frame_duration_;            // 帧间隔时间
    int elapsed_time_;              // 已经度过的时间

    std::vector<QPixmap> frame_list_; // 帧列表
};

#endif // FRAMEANIMATION_H
