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

    const float THUMB_STICK_THRESHOLD = 0.5f;
    const float VELOCITY_MULTIPLIER = 10000.0f;

    void Update(float deltaTime);
    void SetIsPlayButtonClicked();
    bool GetIsPlayButtonClicked() const { return m_isPlayButtonClicked; }
    void ResetPlayButtonClick() { m_isPlayButtonClicked = false; }

private:
    InputHandler() = default;

    bool m_isPlayButtonClicked = false;

    void HandlePositionInput(EntityManager& entityManager, EntityId playerEntityId, float deltaTime);
    void HandleShootingInput(EntityManager& entityManager, EntityId playerEntityId, float deltaTime);
};
