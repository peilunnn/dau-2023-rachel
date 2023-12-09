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

    GameState GetPreviousGameState() const {
        return m_previousGameState;
    }
    GameState GetCurrentGameState() const {
        return m_currentGameState;
    }
    void SetPreviousGameState(GameState newState) {
        m_previousGameState = newState;
    }
    void SetCurrentGameState(GameState newState) {
        m_currentGameState = newState;
    }

private:
    GameManager() = default;

    GameState m_previousGameState = GameState::None;
    GameState m_currentGameState = GameState::Gameplay;
    float m_loadingTimer = 0.0f;
    float m_loadingDuration = 1.0f;
    void UpdateLoadingState(float deltaTime);
};

