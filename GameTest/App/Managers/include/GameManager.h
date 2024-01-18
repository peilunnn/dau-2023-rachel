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
    void ResetGame();

    IGameState* GetCurrentGameState() const { return m_currentGameState.get(); }
    IGameState* GetPreviousGameState() const { return m_previousGameState.get(); }
    bool GetGameReset() const { return m_isGameReset; }
    bool GetIsFirstEnemyInit() const { return m_isFirstEnemyInit; }
    void SetGameReset(bool newIsGameReset) { m_isGameReset = newIsGameReset; }
    void SetIsFirstEnemyInit(bool newIsFirstEnemyInit) { m_isFirstEnemyInit = newIsFirstEnemyInit; }

private:
    GameManager();

    unique_ptr<IGameState> m_currentGameState;
    unique_ptr<IGameState> m_previousGameState;
    bool m_isGameReset = false;
    bool m_isFirstEnemyInit = false;
};

