// effectanimation.cpp
#include "effectanimation.h"
#include "../constants.h"
#include <QDebug>

// 构造函数实现
EffectAnimation::EffectAnimation(std::shared_ptr<Cube> cube, int total_duration_ms)
    : FrameAnimation(cube),
    total_duration_ms_(total_duration_ms),
    initialized_(false)
{
    InitializeFrameDuration();
}

EffectAnimation::EffectAnimation(std::shared_ptr<Cube> cube, QPointF pos, int total_duration_ms)
    : FrameAnimation(cube, pos),
    total_duration_ms_(total_duration_ms),
    initialized_(false)
{
    InitializeFrameDuration();
}

EffectAnimation::EffectAnimation(std::shared_ptr<Cube> cube, QPointF pos, const std::vector<QPixmap> &frame_list, int total_duration_ms)
    : FrameAnimation(cube, pos, frame_list),
    total_duration_ms_(total_duration_ms),
    initialized_(false)
{
    InitializeFrameDuration();
}

// 设置总时长
void EffectAnimation::SetTotalDuration(int total_duration_ms)
{
    total_duration_ms_ = total_duration_ms;
    initialized_ = false; // 需要重新初始化帧持续时间
    InitializeFrameDuration();
}

// 初始化每帧的持续时间
void EffectAnimation::InitializeFrameDuration()
{
    if (frame_list_.empty()) {
        qDebug() << "EffectAnimation: Frame list is empty. Cannot initialize frame duration.";
        return;
    }

    // 计算每帧的持续时间，使总时长覆盖所有帧
    frame_duration_ = total_duration_ms_ / frame_list_.size();
    if (frame_duration_ <= 0) {
        frame_duration_ = 1; // 至少1毫秒
    }
    initialized_ = true;
    qDebug() << "EffectAnimation initialized with total_duration_ms:" << total_duration_ms_
             << " and frame_duration_ms:" << frame_duration_;
}

// 更新动画状态
void EffectAnimation::update(int delta_time)
{
    if (!is_playing_) {
        return;
    }

    if (!initialized_) {
        InitializeFrameDuration();
    }

    elapsed_time_ += delta_time;

    // 检查是否需要切换到下一帧
    while (elapsed_time_ >= frame_duration_) {
        elapsed_time_ -= frame_duration_;
        current_frame_idx_++;

        if (current_frame_idx_ >= static_cast<int>(frame_list_.size())) {
            if (is_loop_) {
                current_frame_idx_ = 0;
            } else {
                is_playing_ = false;
                current_frame_idx_ = frame_list_.size() - 1; // 保持在最后一帧
                qDebug() << "EffectAnimation finished.";
                break;
            }
        }

        qDebug() << "EffectAnimation updated to frame:" << current_frame_idx_;
    }
}

void EffectAnimation::draw(QPainter &painter)
{
    if (!frame_list_.empty() && current_frame_idx_ < static_cast<int>(frame_list_.size())) {
        // 获取当前帧
        const QPixmap &current_pixmap = GetCurrentFrame();

        // 定义目标尺寸，可以根据需要调整
        // 这里假设目标尺寸为 cell_size - 10，留出一些内边距
        QSize targetSize = QSize(Constants::k_cell_size , Constants::k_cell_size);

        // 计算 QPixmap 的缩放尺寸，保持比例并适应目标尺寸
        QPixmap scaled_pixmap = current_pixmap.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // 计算绘制位置，使图片中心对齐到 start_pos_
        int pix_x = static_cast<int>(start_pos_.x() - scaled_pixmap.width() / 2 + Constants::k_cell_size / 2);
        int pix_y = static_cast<int>(start_pos_.y() - scaled_pixmap.height() / 2 + Constants::k_cell_size / 2);
        QPointF draw_pos(static_cast<qreal>(pix_x), static_cast<qreal>(pix_y));

        // 绘制当前帧的 QPixmap
        painter.drawPixmap(draw_pos, scaled_pixmap);
    }
}

