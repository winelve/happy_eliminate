#include "moveanimation.h"
#include "../constants.h"
#include <QPainter>
#include <cmath> // 引入 std::sqrt

// 构造函数实现，接受 std::shared_ptr<Cube>
MoveAnimation::MoveAnimation(std::shared_ptr<Cube> cube)
    : FrameAnimation(cube),
    end_pos_(start_pos_), // 初始化终点为起始位置
    direction_(0.0f, 0.0f),
    speed_(0.0f),
    loop_move_(false),
    delay_frames_(0),
    current_delay_(0),
    delay_active_(false)
{
    is_playing_ = true;
    SetEndDelay(4);
}

// 构造函数，指定起始位置、结束位置、帧列表、速度和是否循环移动
MoveAnimation::MoveAnimation(
    std::shared_ptr<Cube> cube,
    QPointF start_pos,
    QPointF end_pos,
    const std::vector<QPixmap> &frame_list,
    float speed,
    bool loop_move,
    bool loop_frames
    )
    : FrameAnimation(cube, start_pos, frame_list),
    end_pos_(end_pos),
    speed_(speed),
    loop_move_(loop_move),
    delay_frames_(0),
    current_delay_(0),
    delay_active_(false)
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
    SetEndDelay(4);
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


void MoveAnimation::SetEndDelay(int delay_frames)
{
    delay_frames_ = delay_frames;
    current_delay_ = 0;
    delay_active_ = false;

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
                if (cube_) { // 确保 cube_ 不为空
                    cube_->SetPlaying(false);
                }
                break;
            }
        }
    }

    // 如果处于延迟状态，处理延迟
    if (delay_active_) {
        current_delay_++;
        if (current_delay_ >= delay_frames_) {
            // 延迟结束，停止动画
            is_playing_ = false;
            if (cube_) { // 确保 cube_ 不为空
                cube_->SetPlaying(false);
            }
            qDebug() << "Animation delay ended, stopping animation.";
        }
        return; // 在延迟期间，不更新位置
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
                if (delay_frames_ > 0 && use_delay_) {
                    // 启动延迟
                    delay_active_ = true;
                    current_delay_ = 0;
                    qDebug() << "Reached end position, starting delay.";
                } else {
                    // 停止移动
                    is_playing_ = false;
                    if (cube_) { // 确保 cube_ 不为空
                        cube_->SetPlaying(false);
                    }
                    qDebug() << "Reached end position, stopping animation.";
                }
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
        QPointF draw_pos(pix_x, pix_y);

        // 绘制当前帧的 QPixmap
        painter.drawPixmap(draw_pos, scaled_pixmap);
    }
}
