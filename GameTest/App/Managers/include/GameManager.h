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

    void HandlePlayButtonClick();
    void HandleBackButtonClick();

    GameState GetGameState() const {
        return m_gameState;
    }
    void SetGameState(GameState newState) {
        m_gameState = newState;
    }

private:
    GameManager() = default;

    GameState m_gameState = GameState::MainMenu;

    void PrepareGameplay();
};

