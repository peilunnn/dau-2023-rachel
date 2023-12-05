#pragma once
#include "../App/Managers/include/EntityManager.h"
#include "System.h"

class TimerHandler : public System {
public:
    void Update(EntityManager& entityManager, float deltaTime);
};