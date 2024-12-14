#include "animationfactory.h"



AnimationFactory& AnimationFactory::GetInstance()
{
    static AnimationFactory instance;
    return instance;
}


std::shared_ptr<MoveAnimation> AnimationFactory::MakeMoveAnimation(QPointF start, QPointF end, int animal_type,
                                                                   AnimationType texture_type, bool is_loop,
                                                                   float speed)
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

    auto ani = std::make_shared<MoveAnimation>();
    ani->SetLoopMove(false);
    ani->SetSpeed(speed);
    ani->SetLooping(is_loop);
    ani->SetPosition(start.x(), start.y());
    ani->SetEndPosition(end.x(), end.y());
    ani->SetFrameList(ResourceManager::Instance().GetAniResource(animal, texture_type));
    ani->SetPlaying(true);
    return ani;
}

std::shared_ptr<MoveAnimation> AnimationFactory::MakeMoveAnimation(QPointF start, QPointF end,
                                                                   int animal_type, float speed)
{
    auto ani = std::make_shared<MoveAnimation>();
    ani->SetLoopMove(false);
    ani->SetSpeed(speed);
    ani->SetPosition(start.x(), start.y());
    ani->SetEndPosition(end.x(), end.y());
    ani->AddFrame(ResourceManager::Instance().GetCube(animal_type));
    ani->SetPlaying(true);
    return ani;
}




