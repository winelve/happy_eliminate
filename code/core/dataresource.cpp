#include "dataresource.h"

// 静态成员初始化
DataResource* DataResource::instance_ = nullptr;
QMutex DataResource::mutex_;

DataResource::DataResource(QObject* parent)
    : QObject(parent),
    level_(0),
    rest_time_(100),
    score_(0),
    target_score_(0),
    rest_steps_(0) {}

DataResource* DataResource::instance()
{
    if (!instance_) {
        QMutexLocker locker(&mutex_); // 确保线程安全
        if (!instance_) {
            instance_ = new DataResource;
        }
    }
    return instance_;
}

void DataResource::reset_all_data()
{
    QMutexLocker locker(&mutex_); // 确保线程安全

    // 重置所有成员变量
    level_ = 0;
    rest_time_ = 0;
    total_time_ = 0;
    score_ = 0;
    target_score_ = 0;
    rest_steps_ = 0;
    total_steps_ = 0;
    pace_ = 10;
    elimination_times_ = 1;

    // 发射信号更新 UI 或相关逻辑
    emit level_change_signal(level_);
    emit time_change_signal(rest_time_);
    emit score_change_signal(score_);
    emit target_score_change_signal(target_score_);
    emit step_change_signal(rest_steps_);
}
