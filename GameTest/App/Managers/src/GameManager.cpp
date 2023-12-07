#include "stdafx.h"
#include "Components/include/Cooldown.h"
#include "Managers/include/GameManager.h"
#include "Systems/include/InputHandler.h"

void GameManager::Update(float deltaTime)
{
    EntityManager& entityManager = EntityManager::GetInstance();
    InputHandler& inputHandler = InputHandler::GetInstance();

    if (gameState == GameState::MainMenu) {
        if (inputHandler.GetIsPlayButtonClicked()) {
            gameState = GameState::Gameplay;
            inputHandler.ResetPlayButtonClick();
            PrepareGameplay();
        }
    }
    else if (gameState == GameState::Gameplay) {
    }
}

void GameManager::PrepareGameplay() {
    EntityManager& entityManager = EntityManager::GetInstance();
    EntityId playerEntityId = entityManager.GetPlayerEntityId();
    Cooldown* cooldown = entityManager.GetComponent<Cooldown>(playerEntityId);
    cooldown->StartCooldown();
}