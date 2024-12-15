// animationfactory.cpp
#include "animationfactory.h"

AnimationFactory& AnimationFactory::GetInstance()
{
    static AnimationFactory instance;
    return instance;
}

// 修改后的 MakeMoveAnimation 方法，接受 std::shared_ptr<Cube>
std::shared_ptr<MoveAnimation> AnimationFactory::MakeMoveAnimation(
    std::shared_ptr<Cube> cube,
    QPointF start,
    QPointF end,
    int animal_type,
    AnimationType texture_type,
    bool is_loop,
    bool use_delay,
    float speed
    )
{
    AnimalType animal;
    switch (animal_type) {
    case 1: animal = AnimalType::Bear; break;
    case 2: animal = AnimalType::Cat; break;
    case 3: animal = AnimalType::Chicken; break;
    case 4: animal = AnimalType::Fox; break;
    case 5: animal = AnimalType::Frog; break;
    case 6: animal = AnimalType::Horse; break;
    default: animal = AnimalType::Bear; break;
    }

    auto ani = std::make_shared<MoveAnimation>(cube);
    ani->SetLoopMove(false);
    ani->SetSpeed(speed);
    ani->SetLooping(is_loop);
    ani->SetPosition(start.x(), start.y());
    ani->SetEndPosition(end.x(), end.y());
    ani->SetFrameList(ResourceManager::Instance().GetAniResource(animal, texture_type));
    ani->SetPlaying(true);
    ani->SetUseDelay(use_delay);
    return ani;
}

std::shared_ptr<MoveAnimation> AnimationFactory::MakeMoveAnimation(
    std::shared_ptr<Cube> cube,
    QPointF start,
    QPointF end,
    int animal_type,
    bool use_delay,
    float speed
    )
{
    auto ani = std::make_shared<MoveAnimation>(cube);
    ani->SetLoopMove(false);
    ani->SetSpeed(speed);
    ani->SetPosition(start.x(), start.y());
    ani->SetEndPosition(end.x(), end.y());
    ani->AddFrame(ResourceManager::Instance().GetCube(animal_type));
    ani->SetPlaying(true);
    ani->SetUseDelay(use_delay);
    cube->SetPlaying(true);
    return ani;
}


std::shared_ptr<EffectAnimation> AnimationFactory::MakeDestroyEffect(
    std::shared_ptr<Cube> cube,
    QPointF start,
    int ms
    )
{
    std::vector<QPixmap> frames = ResourceManager::Instance().GetDestroyEffect();
    auto ani = std::make_shared<EffectAnimation>(cube,start,frames,ms);
    ani->SetLooping(false);
    ani->SetPlaying(true);
    return ani;
}







