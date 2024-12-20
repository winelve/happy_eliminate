#include "entity.h"

Entity::Entity(QWidget *parent)
    : QWidget{parent},layer_(0)
{}

Entity::Entity(int layer,QWidget *parent)
    : QWidget{parent},layer_(layer)
{}


void Entity::AddAnimation(const QString& state, const std::vector<QPixmap>& frames, int frameDuration, bool loop)
{
    ani_player_.AddAnimation(state,frames,frameDuration,loop);
}

QPropertyAnimation* Entity::CreatMotionAni(Entity* e,QString property,
                                           const QVariant &start_val,
                                           const QVariant &end_val,
                                           const QEasingCurve &easing)
{
    QByteArray property_name = property.toLatin1();
    QPropertyAnimation *ani = new QPropertyAnimation(e,property_name,this->parent()); //请确保有父对象
    ani->setStartValue(start_val);
    ani->setEndValue(end_val);
    ani->setEasingCurve(easing);
    return ani;
}


void Entity::PlayMotionAni(Entity* e,QString property,
                           const QVariant &start_val,
                           const QVariant &end_val,
                           const QEasingCurve &easing)
{
    CreatMotionAni(e,property,start_val,end_val,easing)->start();
}




void Entity::update(int deltaTime)
{
    ani_player_.update(deltaTime);
}

void Entity::render(QPainter &painter)
{
    ani_player_.render(painter,position_);
}


void Entity::SetState(const QString& state)
{
    ani_player_.SetState(state);
}










