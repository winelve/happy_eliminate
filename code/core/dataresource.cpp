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
