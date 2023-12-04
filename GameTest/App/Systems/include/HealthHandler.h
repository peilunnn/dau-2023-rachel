#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"

class HealthHandler : public System {
public:
    void HandleEnemyHitPlayer(EntityManager& entityManager);
    SystemType GetSystemType() const override {
        return SystemType::HealthHandler;
    }

private:
    const int healthReduction = 20;
};