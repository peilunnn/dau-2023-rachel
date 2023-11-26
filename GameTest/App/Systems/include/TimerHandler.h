#ifndef COUNTDOWN_HANDLER_H
#define COUNTDOWN_HANDLER_H

#include "../App/Managers/include/EntityManager.h"
#include "../App/Components/include/Timer.h"
#include "System.h"

class TimerHandler : public System {
public:
    void Update(EntityManager& entityManager, float deltaTime);

    Type GetSystemType() const override {
        return System::Type::TimerHandler;
    }
};

#endif // TIMER_HANDLER_H
