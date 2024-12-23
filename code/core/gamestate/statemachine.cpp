#include "statemachine.h"
#include <QDebug>

StateMachine::StateMachine(QObject *parent)
    : QObject(parent), needInit_(true)
{
}

void StateMachine::RegisterState(const QString &name, std::shared_ptr<StateNode> state)
{
    state->SetStateMachine(this); // 设置状态机
    if (!state) {
        qWarning() << "Cannot register a null state with name:" << name;
        return;
    }

    if (statePool_.contains(name)) {
        qWarning() << "State already registered with name:" << name;
        return;
    }

    statePool_.insert(name, state);
}

void StateMachine::SwitchTo(const QString &name)
{
    if (!statePool_.contains(name)) {
        qWarning() << "State not found:" << name;
        return;
    }

    if (currentState_) {
        currentState_->onExit();
    }

    currentState_ = statePool_.value(name);
    current_state_name_ = name;
    if (currentState_) {
        currentState_->onEnter();
    }

    needInit_ = false;
}

void StateMachine::update()
{
    if (needInit_) {
        qWarning() << "StateMachine not initialized. Call switchTo() first.";
        return;
    }

    if (currentState_) {
        currentState_->onUpdate();
    }
}
