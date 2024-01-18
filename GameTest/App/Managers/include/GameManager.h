#pragma once
#include "Utilities/include/EntityId.h"
#include "States/include/IGameState.h"
using std::unique_ptr;

class GameManager
{
public:
    static GameManager& GetInstance();
    GameManager(GameManager const&) = delete;
    void operator=(GameManager const&) = delete;

    const float COUNTDOWN_DURATION = 60.0f;
    
    void ChangeState(unique_ptr<IGameState> newState);
    void Update(float deltaTime);
    void UpdateCrosshairPosition();
    void HandlePlayButtonClick();
    void HandleBackButtonClick();
    void HandleQuitButtonClick();
    void TransitionToLoadingState();
    void ResetGame();
    void TogglePause();

    IGameState* GetCurrentState() const { return m_currentState.get(); }
    //float GetTimeSpentInLoading() const { return m_timeSpentInLoading; }
    bool GetGameReset() const { return m_isGameReset; }
    bool GetIsFirstEnemyInit() const { return m_isFirstEnemyInit; }
    //void SetPreviousGameState(GameState newState) { m_previousGameState = newState; }
    //void SetCurrentGameState(GameState newState) { m_currentGameState = newState; }
    //void SetTimeSpentInLoading(float newTimeLeftInLoading) { m_timeSpentInLoading = newTimeLeftInLoading; }
    void SetGameReset(bool newIsGameReset) { m_isGameReset = newIsGameReset; }
    void SetIsFirstEnemyInit(bool newIsFirstEnemyInit) { m_isFirstEnemyInit = newIsFirstEnemyInit; }

private:
    GameManager();

    unique_ptr<IGameState> m_currentState;

    //GameState m_previousGameState = GameState::GameOver;
    //GameState m_currentGameState = GameState::MainMenu;
    bool m_isGameReset = false;
    bool m_isFirstEnemyInit = false;

    void UpdateLoadingState(float deltaTime);
    void HandleButtonClick(EntityId buttonEntityId);
};

