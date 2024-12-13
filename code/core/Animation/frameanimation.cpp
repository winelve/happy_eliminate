#include "frameanimation.h"
#include "../constants.h"

FrameAnimation::FrameAnimation()
    : x_(0.0f),
    y_(0.0f),
    is_loop_(true),
    current_frame_idx_(0),
    frame_duration_(Constants::k_duration_time), // 默认每帧100毫秒（即10帧/秒）
    elapsed_time_(0) {}


FrameAnimation::FrameAnimation(float x,float y)
    : x_(x),
    y_(y),
    is_loop_(true),
    current_frame_idx_(0),
    frame_duration_(Constants::k_duration_time), // 默认每帧100毫秒（即10帧/秒）
    elapsed_time_(0) {}

FrameAnimation::FrameAnimation(float x,float y,std::vector<QPixmap> frame_list)
    : x_(x),
    y_(y),
    is_loop_(true),
    current_frame_idx_(0),
    frame_duration_(Constants::k_duration_time), // 默认每帧100毫秒（即10帧/秒）
    elapsed_time_(0)
{
    SetFrameList(frame_list);
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
    x_ = x;
    y_ = y;
}

const QPixmap& FrameAnimation::GetCurrentFrame() const
{
    if (frame_list_.empty()) {
        static QPixmap empty;
        return empty;
    }
    return frame_list_[current_frame_idx_];
}





