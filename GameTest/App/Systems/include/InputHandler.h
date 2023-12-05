#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"
using namespace std;

class InputHandler : public System {
public:
    void Update(EntityManager& entityManager, float deltaTime, EntityId playerEntityId, shared_ptr<CSimpleSprite> bulletSprite);

private:
    static constexpr float THUMB_STICK_THRESHOLD = 0.5f;
    static constexpr float VELOCITY_MULTIPLIER = 10000.0f;

    void HandlePositionInput(EntityManager& entityManager, float deltaTime, EntityId playerEntityId);
    void HandleShootingInput(EntityManager& entityManager, EntityId playerEntityId, shared_ptr<CSimpleSprite> bulletSprite);
};