#pragma once
#include "../../Managers/include/EntityManager.h"
#include "../../Systems/include/System.h"
#include "../../Utilities/include/Helper.h"

class ScoreHandler : public System
{
public:
    void ProcessBulletHitEnemy(EntityManager &entityManager, float deltaTime);
    SystemType GetSystemType() const override
    {
        return SystemType::ScreenHandler;
    }
};