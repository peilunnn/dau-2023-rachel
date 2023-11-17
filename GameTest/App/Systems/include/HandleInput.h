#ifndef HANDLE_INPUT_H
#define HANDLE_INPUT_H

#include "../../Entities/include/EntityManager.h"
#include "../../Components/include/Velocity.h"
#include "../../Application/include/app.h"

class HandleInput {
public:
    void Update(EntityManager& entityManager, float deltaTime, Entity playerEntityId);
    static constexpr float THUMB_STICK_THRESHOLD = 0.5f;
    static constexpr float VELOCITY_MULTIPLIER = 0.08f;
};

#endif // HANDLE_INPUT_H
