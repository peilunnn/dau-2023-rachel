#pragma once
#include "Utilities/include/Enums.h"

class GameManager
{
public:
    static GameManager& GetInstance();
    GameManager(GameManager const&) = delete;
    void operator=(GameManager const&) = delete;

    void Update(float deltaTime);
    void HandlePlayButtonClick();
    void HandleBackButtonClick();
    void HandleQuitButtonClick();
    void TransitionToLoadingState();
    void ResetGame();
    void TogglePause();

    GameState GetPreviousGameState() const {
        return m_previousGameState;
    }
    GameState GetCurrentGameState() const {
        return m_currentGameState;
    }
    float GetTimeSpentInLoading() const {
        return m_timeSpentInLoading;
    }
    void SetPreviousGameState(GameState newState) {
        m_previousGameState = newState;
    }
    void SetCurrentGameState(GameState newState) {
        m_currentGameState = newState;
    }
    void SetTimeSpentInLoading(float newTimeLeftInLoading) {
        m_timeSpentInLoading = newTimeLeftInLoading;
    }

private:
    GameManager() = default;

    GameState m_previousGameState = GameState::GameOver;
    GameState m_currentGameState = GameState::MainMenu;
    float m_timeSpentInLoading = 0.0f;
    float m_loadingDuration = 1.5f;
    void UpdateLoadingState(float deltaTime);
};

