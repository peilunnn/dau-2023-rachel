#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"

class ScoreHandler : public System
{
public:
    void ProcessBulletHitEnemy(EntityManager &entityManager, float deltaTime);
    SystemType GetSystemType() const override
    {
        return SystemType::ScreenHandler;
    }
};