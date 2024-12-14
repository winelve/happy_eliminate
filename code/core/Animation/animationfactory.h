#ifndef ANIMATIONFACTORY_H
#define ANIMATIONFACTORY_H

#include "resourcemanager.h"
#include "moveanimation.h"
#include "../constants.h"

class AnimationFactory
{
public:

    // 删除拷贝构造函数和赋值运算符
    AnimationFactory(const AnimationFactory&) = delete;
    AnimationFactory(AnimationFactory&&) = delete;


    static AnimationFactory& GetInstance();
    std::shared_ptr<MoveAnimation> MakeMoveAnimation(QPointF start,QPointF end,int animal_type,AnimationType texture_type,bool is_loop,float speed=Constants::move_speed);
    std::shared_ptr<MoveAnimation> MakeMoveAnimation(QPointF start,QPointF end,int animal_type,float speed=Constants::move_speed);

private:
    // 私有构造函数，防止外部实例化
    AnimationFactory() = default;
    // 可选：析构函数也可以私有化
    ~AnimationFactory() = default;

};

#endif // ANIMATIONFACTORY_H
