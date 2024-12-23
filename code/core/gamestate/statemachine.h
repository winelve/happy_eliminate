#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "statenode.h"
#include <QObject>
#include <QMap>
#include <memory>

class StateMachine : public QObject
{
    Q_OBJECT

private:
    std::shared_ptr<StateNode> currentState_;
    QMap<QString, std::shared_ptr<StateNode>> statePool_;
    bool needInit_;

public:
    explicit StateMachine(QObject *parent = nullptr);
    ~StateMachine() override = default;

    void RegisterState(const QString &name, std::shared_ptr<StateNode> state);
    void SwitchTo(const QString &name);
    void update();
};

#endif // STATEMACHINE_H

