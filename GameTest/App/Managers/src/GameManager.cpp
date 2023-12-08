#include "stdafx.h"
#include "Components/include/Cooldown.h"
#include "Managers/include/GameManager.h"
#include "Systems/include/InputHandler.h"
#include "Utilities/include/Helper.h"

void GameManager::HandlePlayButtonClick()
{
    InputHandler& inputHandler = InputHandler::GetInstance();

    if (!inputHandler.GetIsPlayButtonClicked())
        return;

    m_gameState = GameState::Gameplay;
    inputHandler.ResetPlayButtonClick();
    PrepareGameplay();
}

void GameManager::HandleBackButtonClick()
{
    InputHandler& inputHandler = InputHandler::GetInstance();

    if (!inputHandler.GetIsBackButtonClicked())
        return;

    m_gameState = GameState::MainMenu;
    inputHandler.ResetBackButtonClick();
}

void GameManager::PrepareGameplay() {
    EntityManager& entityManager = EntityManager::GetInstance();
    EntityId playerEntityId = entityManager.GetPlayerEntityId();
    Cooldown* cooldown = entityManager.GetComponent<Cooldown>(playerEntityId);
    cooldown->StartCooldown();
}