#ifndef ENTITY_H
#define ENTITY_H

#include <QWidget>
#include <QString>
#include <QPainter>
#include <QPropertyAnimation>

#include "Animation/animationcomponent.h"

//可以播放动画的实体类
class Entity : public QWidget
{
    Q_OBJECT
public:
    explicit Entity(QWidget *parent = nullptr);
    explicit Entity(int layer,QWidget *parent = nullptr);

    // 去注册一个新的动画
    void AddAnimation(const QString& state, const std::vector<QPixmap>& frames, int frameDuration, bool loop = true);
    //创建一个额外的运动动画
    QPropertyAnimation *CreatMotionAni(Entity* e,QString property,const QVariant &start_val,const QVariant &end_val,const QEasingCurve &easing = QEasingCurve::Linear);
    void PlayMotionAni(Entity* e,QString property,const QVariant &start_val,const QVariant &end_val,const QEasingCurve &easing = QEasingCurve::Linear); //直接播放对应的动画
    void SetState(const QString& state); //切换状态
    virtual void update(int deltaTime); //毫秒计时
    virtual void render(QPainter &painter);

    void SetPos(QPointF pos) { position_ = pos; }
    QPointF GetPos() { return position_; }
    bool IsFinished() {return ani_player_.finished(); }
    int GetLayer() { return layer_; };
    void SetLayer(int layer) { layer_ = layer; }

private:
    QPointF position_;
    AnimationComponent ani_player_;
    int layer_;
};

#endif // ENTITY_H
