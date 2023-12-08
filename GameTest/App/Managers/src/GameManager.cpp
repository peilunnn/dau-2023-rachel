#include "stdafx.h"
#include "Components/include/Cooldown.h"
#include "Managers/include/GameManager.h"
#include "Systems/include/InputHandler.h"
#include "Utilities/include/Helper.h"

void GameManager::Update(float deltaTime)
{
    switch (m_gameState)
    {
    case GameState::Loading:
        UpdateLoadingState(deltaTime);
        break;
    }
}

void GameManager::UpdateLoadingState(float deltaTime)
{
    m_loadingTimer += deltaTime;
    if (m_loadingTimer >= m_loadingDuration)
    {
        if (m_previousGameState == GameState::MainMenu)
            m_gameState = GameState::Gameplay;
        else if (m_previousGameState == GameState::GameOver)
            m_gameState = GameState::MainMenu;
    }
}

void GameManager::HandlePlayButtonClick()
{
    InputHandler& inputHandler = InputHandler::GetInstance();

    if (!inputHandler.GetIsPlayButtonClicked())
        return;

    m_previousGameState = m_gameState;
    m_gameState = GameState::Loading;
    m_loadingTimer = 0.0f;
    inputHandler.ResetPlayButtonClick();
}

void GameManager::HandleBackButtonClick()
{
    InputHandler& inputHandler = InputHandler::GetInstance();

    if (!inputHandler.GetIsBackButtonClicked())
        return;
    
    m_previousGameState = m_gameState;
    m_gameState = GameState::Loading;
    m_loadingTimer = 0.0f;
    inputHandler.ResetBackButtonClick();
}

void GameManager::TransitionToLoadingState()
{
    m_previousGameState = m_gameState;
    m_gameState = GameState::Loading;
    m_loadingTimer = 0.0f;
}