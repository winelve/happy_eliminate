// frameanimation.cpp
#include "frameanimation.h"
#include "../constants.h"

// 构造函数实现，接受 std::shared_ptr<Cube>
FrameAnimation::FrameAnimation(std::shared_ptr<Cube> cube)
    : cube_(cube),
    start_pos_(),
    is_loop_(true),
    is_playing_(false),
    current_frame_idx_(0),
    frame_duration_(Constants::k_duration_time), // 默认每帧100毫秒（即10帧/秒）
    elapsed_time_(0)
{
}

FrameAnimation::FrameAnimation(std::shared_ptr<Cube> cube, QPointF pos)
    : cube_(cube),
    start_pos_(pos),
    is_loop_(true),
    is_playing_(false),
    current_frame_idx_(0),
    frame_duration_(Constants::k_duration_time), // 默认每帧100毫秒（即10帧/秒）
    elapsed_time_(0)
{
}

FrameAnimation::FrameAnimation(std::shared_ptr<Cube> cube, QPointF pos, std::vector<QPixmap> frame_list)
    : cube_(cube),
    start_pos_(pos),
    is_loop_(true),
    is_playing_(false),
    current_frame_idx_(0),
    frame_duration_(Constants::k_duration_time), // 默认每帧100毫秒（即10帧/秒）
    elapsed_time_(0),
    frame_list_(frame_list)
{
}

void FrameAnimation::AddFrame(const QPixmap &img)
{
    frame_list_.emplace_back(img);
}

void FrameAnimation::AddFrameList(const std::vector<QPixmap> &frames)
{
    frame_list_.insert(frame_list_.end(), frames.begin(), frames.end());
}

void FrameAnimation::SetFrameList(const std::vector<QPixmap> &frames)
{
    frame_list_ = frames;
    current_frame_idx_ = 0;
    elapsed_time_ = 0;
}

void FrameAnimation::SetLooping(bool loop)
{
    is_loop_ = loop;
}

void FrameAnimation::SetFrameDuration(int ms)
{
    frame_duration_ = ms;
}

void FrameAnimation::SetPosition(float x, float y)
{
    start_pos_.setX(x);
    start_pos_.setY(y);
}

const QPixmap& FrameAnimation::GetCurrentFrame() const
{
    if (frame_list_.empty()) {
        static QPixmap empty;
        return empty;
    }
    return frame_list_[current_frame_idx_];
}
