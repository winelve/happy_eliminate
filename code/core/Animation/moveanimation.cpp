#include "moveanimation.h"
#include "../constants.h"
#include <QPainter>


MoveAnimation::MoveAnimation()
    : FrameAnimation(),
    end_pos_(start_pos_),
    direction_(0.0f, 0.0f),
    speed_(0.0f),
    loop_move_(false)
{
}

MoveAnimation::MoveAnimation(QPointF start_pos, QPointF end_pos, const std::vector<QPixmap> &frame_list, float speed, bool loop_move, bool loop_frames)
    : FrameAnimation(start_pos, frame_list),
    end_pos_(end_pos),
    speed_(speed),
    loop_move_(loop_move)
{
    SetLooping(loop_frames);
    SetFrameDuration(frame_duration_);

    // 计算方向向量并归一化
    QPointF delta = end_pos_ - start_pos_;
    qreal length = std::sqrt(delta.x() * delta.x() + delta.y() * delta.y());
    if (length != 0.0) {
        direction_ = QPointF(delta.x() / length, delta.y() / length);
    } else {
        direction_ = QPointF(0.0f, 0.0f);
    }

    is_playing_ = true;
}


void MoveAnimation::SetEndPosition(float x, float y)
{
    end_pos_ = QPointF(x, y);
    QPointF delta = end_pos_ - start_pos_;
    qreal length = std::sqrt(delta.x() * delta.x() + delta.y() * delta.y());
    if (length != 0.0) {
        direction_ = QPointF(delta.x() / length, delta.y() / length);
    } else {
        direction_ = QPointF(0.0f, 0.0f);
    }
}

void MoveAnimation::SetSpeed(float speed)
{
    speed_ = speed;
}

void MoveAnimation::SetLoopMove(bool loop_move)
{
    loop_move_ = loop_move;
}

void MoveAnimation::update(int delta_time)
{
    if (!is_playing_)
        return;

    // 更新帧动画
    elapsed_time_ += delta_time;

    while (elapsed_time_ >= frame_duration_) {
        elapsed_time_ -= frame_duration_;
        current_frame_idx_++;
        if (current_frame_idx_ >= static_cast<int>(frame_list_.size())) {
            if (is_loop_) {
                current_frame_idx_ = 0;
            } else {
                current_frame_idx_ = static_cast<int>(frame_list_.size()) - 1;
                is_playing_ = false;
                break;
            }
        }
    }

    // 更新位置
    if (speed_ > 0.0f) {
        // 计算移动距离
        float distance = speed_ * (delta_time / 1000.0f); // 像素

        // 计算新的位置
        QPointF delta_move = direction_ * distance;
        QPointF new_pos = start_pos_ + delta_move;

        // 检查是否到达或超过终点
        bool reached_end = false;
        if ((direction_.x() > 0 && new_pos.x() >= end_pos_.x()) ||
            (direction_.x() < 0 && new_pos.x() <= end_pos_.x())) {
            new_pos.setX(end_pos_.x());
            reached_end = true;
        }

        if ((direction_.y() > 0 && new_pos.y() >= end_pos_.y()) ||
            (direction_.y() < 0 && new_pos.y() <= end_pos_.y())) {
            new_pos.setY(end_pos_.y());
            reached_end = true;
        }

        SetPosition(new_pos.x(), new_pos.y());

        if (reached_end) {
            if (loop_move_) {
                // 重置到起始位置
                SetPosition(start_pos_.x(), start_pos_.y());
            } else {
                // 停止移动
                is_playing_ = false;
            }
        }
    }
}


void MoveAnimation::draw(QPainter &painter)
{
    if (!frame_list_.empty()) {
        // 获取当前帧
        const QPixmap &current_pixmap = GetCurrentFrame();

        // 定义目标尺寸，可以根据需要调整
        // 这里假设目标尺寸为 80x80 像素
        QSize targetSize = QSize(Constants::k_cell_size - 10, Constants::k_cell_size - 10); // 留出一些内边距

        // 计算 QPixmap 的缩放尺寸，保持比例并适应目标尺寸
        QPixmap scaled_pixmap = current_pixmap.scaled(targetSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        int pix_x = start_pos_.x() + (Constants::k_cell_size - scaled_pixmap.width()) / 2;
        int pix_y = start_pos_.y() + (Constants::k_cell_size - scaled_pixmap.height()) / 2;
        // 计算绘制位置，使图片中心对齐到 start_pos_
        QPointF draw_pos(pix_x,pix_y);

        // 绘制当前帧的 QPixmap
        painter.drawPixmap(draw_pos, scaled_pixmap);
    }
}
