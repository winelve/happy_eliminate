#ifndef FRAMEANIMATION_H
#define FRAMEANIMATION_H

#include <vector>
#include <QRect>
#include <QPixmap>


class FrameAnimation
{
public:
    FrameAnimation();
    FrameAnimation(QPointF pos);
    FrameAnimation(QPointF pos,std::vector<QPixmap> frame_list);

    void AddFrame(const QPixmap &img);
    void AddFrameList(const std::vector<QPixmap> &frame_list);
    void SetFrameList(const std::vector<QPixmap> &frame_list);

    const QPixmap& GetCurrentFrame() const;// 获取当前帧
    void SetLooping(bool loop);// 设置是否循环
    void SetFrameDuration(int ms); // 设置每帧持续时间（毫秒）
    void SetPosition(float x, float y); // 设置动画位置
    bool IsPlaying() { return is_playing_; }
    void SetPlaying(bool playing) { is_playing_ = playing; }

    virtual void update(int delta_time) = 0; //ms
    virtual void draw(QPainter &painter) = 0;
protected:

    //动画的坐标位置
    QPointF start_pos_;

    bool is_loop_; //是否是循环
    bool is_playing_; //是否正在播放
    int current_frame_idx_; //当前帧的索引
    int frame_duration_; // 帧间隔
    int elapsed_time_; //已经度过的间隔时间

    std::vector<QPixmap> frame_list_;
};

#endif // FRAMEANIMATION_H
