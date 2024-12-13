#include "atlas.h"
#include <QDebug>

Atlas::Atlas() {}


void Atlas::Clear(){
    atlas_list.clear();
}

int Atlas::GetSize(){
    return atlas_list.size();
}

QPixmap* Atlas::GetAtlas(int idx){
    if(idx<0 || idx >= GetSize()){
        return nullptr;
    }
    return &atlas_list[idx];
}

void Atlas::AddAtlas(const QPixmap &frame){
    atlas_list.push_back(frame);
}


void Atlas::Load(const QString& path, int num) {
    // 清理之前的帧数据
    Clear();

    // 调整atlas_list的大小
    atlas_list.resize(num);

    // 从资源文件中加载图片
    QPixmap spriteSheet(path);
    if (spriteSheet.isNull()) {
        qDebug() << "Failed to load image from path:" << path;
        return;
    }
    // 获取图片的宽度和高度
    int totalWidth = spriteSheet.width();
    int height = spriteSheet.height();
    // 计算每一帧的宽度
    if (num <= 0) {
        qDebug() << "Number of frames must be positive.";
        return;
    }
    // 检查图片宽度是否可以被num整除
    if (totalWidth % num != 0) {
        qDebug() << "Warning: The image width is not evenly divisible by the number of frames.";
    }
    int frameWidth = totalWidth / num;
    // 分割图片并存储到atlas_list
    for (int i = 0; i < num; ++i) {
        // 定义每一帧的矩形区域
        QRect frameRect(i * frameWidth, 0, frameWidth, height);
        // 如果是最后一帧，可能需要调整宽度以包含剩余的像素
        if (i == num - 1) {
            frameRect.setWidth(totalWidth - i * frameWidth);
        }
        // 拷贝每一帧
        QPixmap frame = spriteSheet.copy(frameRect);
        if (frame.isNull()) {
            qDebug() << "Failed to copy frame" << i;
            continue;
        }
        // 存储到atlas_list
        atlas_list[i] = frame;
    }
    qDebug() << "Successfully loaded" << num << "frames from" << path;
}









