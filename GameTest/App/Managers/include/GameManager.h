#pragma once
#include "Utilities/include/Enums.h"

class GameManager
{
public:
    static GameManager& GetInstance();
    GameManager(GameManager const&) = delete;
    void operator=(GameManager const&) = delete;

    void Update(float deltaTime);
    void UpdateCrosshairPosition();
    void HandlePlayButtonClick();
    void HandleBackButtonClick();
    void HandleQuitButtonClick();
    void TransitionToLoadingState();
    void ResetGame();
    void TogglePause();

    GameState GetPreviousGameState() const { return m_previousGameState; }
    GameState GetCurrentGameState() const { return m_currentGameState; }
    float GetTimeSpentInLoading() const { return m_timeSpentInLoading; }
    bool GetGameReset() const { return m_isGameReset; }
    bool GetIsFirstEnemyInit() const { return m_isFirstEnemyInit; }
    void SetPreviousGameState(GameState newState) { m_previousGameState = newState; }
    void SetCurrentGameState(GameState newState) { m_currentGameState = newState; }
    void SetTimeSpentInLoading(float newTimeLeftInLoading) { m_timeSpentInLoading = newTimeLeftInLoading; }
    void SetGameReset(bool newIsGameReset) { m_isGameReset = newIsGameReset; }
    void SetIsFirstEnemyInit(bool newIsFirstEnemyInit) { m_isFirstEnemyInit = newIsFirstEnemyInit; }

private:
    GameManager() = default;

    GameState m_previousGameState = GameState::GameOver;
    GameState m_currentGameState = GameState::MainMenu;
    float m_timeSpentInLoading = 0.0f;
    float m_loadingDuration = 1.5f;
    bool m_isGameReset = false;
    bool m_isFirstEnemyInit = false;

    void UpdateLoadingState(float deltaTime);
};

