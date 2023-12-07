#include "stdafx.h"
#include "Managers/include/GameManager.h"
#include "Systems/include/InputHandler.h"

void GameManager::Update(float deltaTime)
{
    InputHandler& inputHandler = InputHandler::GetInstance();

    if (gameState == GameState::MainMenu) {
        if (inputHandler.GetIsPlayButtonClicked()) {
            gameState = GameState::Gameplay;
            inputHandler.ResetPlayButtonClick();
        }
    }
    else if (gameState == GameState::Gameplay) {
    }
}
