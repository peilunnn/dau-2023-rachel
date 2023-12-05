#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"
using namespace std;

class InputHandler : public System {
public:
    static InputHandler& GetInstance() {
        static InputHandler instance;
        return instance;
    }
    InputHandler(InputHandler const&) = delete;
    void operator=(InputHandler const&) = delete;

    void Update(float deltaTime, EntityId playerEntityId, shared_ptr<CSimpleSprite> bulletSprite);

private:
    InputHandler() = default;

    static constexpr float THUMB_STICK_THRESHOLD = 0.5f;
    static constexpr float VELOCITY_MULTIPLIER = 10000.0f;

    void HandlePositionInput(float deltaTime, EntityId playerEntityId);
    void HandleShootingInput(EntityManager& entityManager, EntityId playerEntityId, shared_ptr<CSimpleSprite> bulletSprite);
};
