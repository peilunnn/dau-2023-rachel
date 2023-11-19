#ifndef HANDLE_INPUT_H
#define HANDLE_INPUT_H

#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Velocity.h"
#include "../../Systems/include/HandleShooting.h"
#include "../../Utilities/include/app.h"
#include "../../Systems/include/System.h"

class HandleInput : public System
{
public:
    static constexpr float THUMB_STICK_THRESHOLD = 0.5f;
    static constexpr float VELOCITY_MULTIPLIER = 0.08f;
    static constexpr float ANGLE_DELTA = 0.1f;
    static constexpr float SCALE_DELTA = 0.1f;

    void Update(EntityManager& entityManager, float deltaTime, Entity playerEntityId, CSimpleSprite* bulletSprite);
    void HandlePositionInput(EntityManager& entityManager, float deltaTime, Entity playerEntityId);
    void HandleRotationInput(EntityManager& entityManager, Entity playerEntityId);
    void HandleScaleInput(EntityManager& entityManager, Entity playerEntityId);
    void HandleShootingInput(EntityManager& entityManager, Entity playerEntityId, CSimpleSprite* bulletSprite);
    Type GetSystemType() const override {
        return System::Type::HandleInput;
    }
};

#endif // HANDLE_INPUT_H