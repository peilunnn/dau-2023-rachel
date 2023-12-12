#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"
using namespace std;

class InputHandler : public System {
public:
    static InputHandler& GetInstance();
    InputHandler(InputHandler const&) = delete;
    void operator=(InputHandler const&) = delete;

    const float THUMB_STICK_THRESHOLD = 0.5f;
    const float VELOCITY_MULTIPLIER = 10000.0f;

    void Update(float deltaTime);
    bool GetIsPlayButtonClicked() const { return m_isPlayButtonClicked; }
    bool GetIsBackButtonClicked() const { return m_isBackButtonClicked; }
    bool GetIsQuitButtonClicked() const { return m_isQuitButtonClicked; }
    void SetIsPlayButtonClicked();
    void SetIsBackButtonClicked();
    void SetIsQuitButtonClicked();
    void ResetPlayButtonClick() { m_isPlayButtonClicked = false; }
    void ResetBackButtonClick() { m_isBackButtonClicked = false; }
    void ResetQuitButtonClick() { m_isQuitButtonClicked = false; }

private:
    InputHandler() = default;

    bool m_isPlayButtonClicked = false;
    bool m_isBackButtonClicked = false;
    bool m_isQuitButtonClicked = false;
    bool m_wasPPressedLastFrame = false;

    void HandlePositionInput(EntityManager& entityManager, EntityId playerEntityId, float deltaTime);
    void HandleShootingInput(EntityManager& entityManager, EntityId playerEntityId, float deltaTime);
    void HandlePauseInput();
};
