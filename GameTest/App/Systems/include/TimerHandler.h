#pragma once
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