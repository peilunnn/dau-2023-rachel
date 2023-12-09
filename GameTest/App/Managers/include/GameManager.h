#pragma once
#include "Utilities/include/Enums.h"

class GameManager
{
public:
    static GameManager& GetInstance() {
        static GameManager instance;
        return instance;
    }
    GameManager(GameManager const&) = delete;
    void operator=(GameManager const&) = delete;

    void Update(float deltaTime);
    void HandlePlayButtonClick();
    void HandleBackButtonClick();

    void TransitionToLoadingState();

    GameState GetGameState() const {
        return m_gameState;
    }
    void SetGameState(GameState newState) {
        m_gameState = newState;
    }

private:
    GameManager() = default;

    GameState m_previousGameState = GameState::Gameplay;
    GameState m_gameState = GameState::MainMenu;
    float m_loadingTimer = 0.0f;
    float m_loadingDuration = 1.0f;
    void UpdateLoadingState(float deltaTime);
};

