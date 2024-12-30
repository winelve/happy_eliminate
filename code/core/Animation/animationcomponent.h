#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include <QObject>
#include <QPixmap>
#include <QMap>
#include <QPainter>
#include <QString>
#include <vector>
#include "../utils/constants.h"

class AnimationComponent : public QObject {
    Q_OBJECT

public:
    explicit AnimationComponent(QObject* parent = nullptr);
    // 添加动画
    void AddAnimation(const QString& state, const std::vector<QPixmap>& frames, int frameDuration, bool loop = true);
    // 设置当前动画状态
    void SetState(const QString& state);
    // 更新动画
    void update(int deltaTime);
    // 渲染当前动画帧
    void render(QPainter& painter, const QPointF& position,const qreal opacity = 1,const QSize &target_size=Constants::k_cube_size);
    //是否完成
    bool finished() { return !playing; }
private:
    struct AnimationData {
        std::vector<QPixmap> frames; // 动画帧
        int frameDuration;           // 每帧持续时间（毫秒）
        bool loop;                   // 是否循环播放
    };
    QMap<QString, AnimationData> animations; // 状态到动画数据的映射
    QString currentState;                    // 当前动画状态
    int elapsedTime;                         // 当前帧累计时间（毫秒）
    size_t currentFrameIndex;                // 当前帧索引
    bool playing;                            // 是否正在播放

};

#endif // ANIMATIONCOMPONENT_H
