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

    GameState GetGameState() const {
        return gameState;
    }
    void SetGameState(GameState newState) {
        gameState = newState;
    }

private:
    GameManager() = default;

    GameState gameState = GameState::MainMenu;
};

