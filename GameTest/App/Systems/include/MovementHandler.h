#pragma once
#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Velocity.h"
#include "../../Components/include/Direction.h"
#include "../../Components/include/Tag.h"
#include "../../Utilities/include/Helper.h"
#include "../../Systems/include/System.h"
#include "../../Systems/include/ScreenHandler.h"

class MovementHandler : public System {
public:
    void Update(EntityManager& entityManager, float deltaTime);
    void HandlePlayerMovement(EntityManager& entityManager, Entity entity, float deltaTime);
    void HandleEnemyMovement(EntityManager& entityManager, Entity entity, float deltaTime);
    void HandleBulletMovement(EntityManager& entityManager, Entity entity, float deltaTime);
    Type GetSystemType() const override {
        return System::Type::MovementHandler;
    }
};