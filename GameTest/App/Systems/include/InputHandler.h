#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"
class InputHandler : public System 
{
public:
    static InputHandler& GetInstance();
    InputHandler(InputHandler const&) = delete;
    void operator=(InputHandler const&) = delete;

    const float THUMB_STICK_THRESHOLD = 0.5f;
    const float VELOCITY_MULTIPLIER = 10000.0f;

    void Update(float deltaTime);
    bool IsButtonClicked(EntityId entityId);

private:
    InputHandler() = default;

    bool m_wasPPressedLastFrame = false;

    void HandlePositionInput(EntityManager& entityManager, EntityId playerEntityId, float deltaTime);
    void HandleShootingInput(EntityManager& entityManager, EntityId playerEntityId, float deltaTime);
    void HandlePauseInput();
};
