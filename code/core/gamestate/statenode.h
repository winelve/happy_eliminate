#ifndef STATENODE_H
#define STATENODE_H

#include <QObject>
#include <QDebug>

class StateMachine;

class StateNode : public QObject
{
    Q_OBJECT

public:
    explicit StateNode(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~StateNode() = default;

    virtual void onEnter() { qDebug()<< "StateNode::default enter"; }
    virtual void onExit() { qDebug()<< "StateNode::default exit"; }
    virtual void onUpdate() { qDebug()<< "StateNode::default update"; }
    void SetStateMachine(StateMachine* stateMachine) { this->state_machine_ = stateMachine; }
protected:
    StateMachine* state_machine_;


};

#endif // STATENODE_H
