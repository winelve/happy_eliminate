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

    auto ani = std::make_shared<MoveAnimation>(cube);
    ani->SetLoopMove(false);
    ani->SetSpeed(speed);
    ani->SetLooping(is_loop);
    ani->SetPosition(start.x(), start.y());
    ani->SetEndPosition(end.x(), end.y());
    ani->SetFrameList(ResourceManager::Instance().GetAniResource(GetAniType(animal_type), texture_type));
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

std::shared_ptr<EffectAnimation> AnimationFactory::MakeCubeAnimation(
    std::shared_ptr<Cube> cube,
    QPointF start,
    AnimationType ani_type,
    bool is_loop
    )
{
    std::vector<QPixmap> frames = ResourceManager::Instance().GetAniResource(GetAniType(cube->GetType()),ani_type);
    auto ani = std::make_shared<EffectAnimation>(cube,start,frames);
    ani->SetLooping(is_loop);
    ani->SetPlaying(true);
    cube->SetPlaying(true);
    return ani;
}



std::shared_ptr<MoveAnimation> AnimationFactory::MakeMoveAnimation(
    std::shared_ptr<Cube> cube,
    std::vector<QPixmap> frames,
    QPointF start,
    QPointF end,
    bool is_loop,
    bool use_delay,
    float speed
    )
{

    auto ani = std::make_shared<MoveAnimation>(cube);
    ani->SetLoopMove(false);
    ani->SetSpeed(speed);
    ani->SetLooping(is_loop);
    ani->SetPosition(start.x(), start.y());
    ani->SetEndPosition(end.x(), end.y());
    ani->SetFrameList(frames);
    ani->SetPlaying(true);
    ani->SetUseDelay(use_delay);
    return ani;
}

std::shared_ptr<MoveAnimation> AnimationFactory::MakeMoveAnimation(
    std::shared_ptr<Cube> cube,
    QPixmap frame,
    QPointF start,
    QPointF end,
    bool is_loop,
    bool use_delay,
    float speed,
    bool auto_fit
    )
{

    auto ani = std::make_shared<MoveAnimation>(cube);
    ani->SetLoopMove(false);
    ani->SetSpeed(speed);
    ani->SetLooping(is_loop);
    ani->SetPosition(start.x(), start.y());
    ani->SetEndPosition(end.x(), end.y());
    ani->AddFrame(frame);
    ani->SetPlaying(true);
    ani->SetUseDelay(use_delay);
    ani->SetAutoFit(auto_fit);
    return ani;
}



