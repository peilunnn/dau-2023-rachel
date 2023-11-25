#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Velocity.h"
#include "../../Systems/include/ShootingHandler.h"
#include "../../Utilities/include/app.h"
#include "../../Systems/include/System.h"

class InputHandler : public System
{
public:
    static constexpr float THUMB_STICK_THRESHOLD = 0.5f;
    static constexpr float VELOCITY_MULTIPLIER = 80.0f;
    static constexpr float ANGLE_DELTA = 0.1f;
    static constexpr float SCALE_DELTA = 0.1f;

    void Update(EntityManager& entityManager, float deltaTime, Entity playerEntity, std::shared_ptr<CSimpleSprite> bulletSprite);
    void HandlePositionInput(EntityManager& entityManager, float deltaTime, Entity playerEntity);
    void HandleRotationInput(EntityManager& entityManager, Entity playerEntity);
    void HandleScaleInput(EntityManager& entityManager, Entity playerEntity);
    void HandleShootingInput(EntityManager& entityManager, Entity playerEntity, std::shared_ptr<CSimpleSprite> bulletSprite);
    Type GetSystemType() const override {
        return System::Type::InputHandler;
    }
};

#endif // INPUT_HANDLER_H