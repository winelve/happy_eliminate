// animationfactory.h
#ifndef ANIMATIONFACTORY_H
#define ANIMATIONFACTORY_H

#include <memory>
#include "resourcemanager.h"
#include "moveanimation.h"
#include "effectanimation.h"
#include "../constants.h"

class AnimationFactory
{
public:

    // 删除拷贝构造函数和赋值运算符
    AnimationFactory(const AnimationFactory&) = delete;
    AnimationFactory(AnimationFactory&&) = delete;

    static AnimationFactory& GetInstance();

    // 修改为接受 std::shared_ptr<Cube>
    std::shared_ptr<MoveAnimation> MakeMoveAnimation(
        std::shared_ptr<Cube> cube,
        QPointF start,
        QPointF end,
        int animal_type,
        AnimationType texture_type,
        bool is_loop,
        bool use_delay,
        float speed = Constants::move_speed
        );

    std::shared_ptr<MoveAnimation> MakeMoveAnimation(
        std::shared_ptr<Cube> cube,
        QPointF start,
        QPointF end,
        int animal_type,
        bool use_delay,
        float speed = Constants::move_speed
        );

    std::shared_ptr<EffectAnimation> MakeDestroyEffect(
        std::shared_ptr<Cube> cube,
        QPointF start,
        int ms = 250
    );

    std::shared_ptr<EffectAnimation> MakeCubeAnimation(
        std::shared_ptr<Cube> cube,
        QPointF start,
        AnimationType ani_type,
        bool is_loop
    );

    AnimalType GetAniType(int type){
        AnimalType animal;
        switch (type) {
        case 1: animal = AnimalType::Bear; break;
        case 2: animal = AnimalType::Cat; break;
        case 3: animal = AnimalType::Chicken; break;
        case 4: animal = AnimalType::Fox; break;
        case 5: animal = AnimalType::Frog; break;
        case 6: animal = AnimalType::Horse; break;
        default: animal = AnimalType::Bear; break;
        }
        return animal;
    }

    std::shared_ptr<MoveAnimation> MakeMoveAnimation(
        std::shared_ptr<Cube> cube,
        std::vector<QPixmap> frames,
        QPointF start,
        QPointF end,
        bool is_loop,
        bool use_delay,
        float speed
    );

    std::shared_ptr<MoveAnimation> MakeMoveAnimation(
        std::shared_ptr<Cube> cube,
        QPixmap frame,
        QPointF start,
        QPointF end,
        bool is_loop,
        bool use_delay,
        float speed,
        bool auto_fit = true
        );


private:
    // 私有构造函数，防止外部实例化
    AnimationFactory() = default;
    // 可选：析构函数也可以私有化
    ~AnimationFactory() = default;
};

#endif // ANIMATIONFACTORY_H
