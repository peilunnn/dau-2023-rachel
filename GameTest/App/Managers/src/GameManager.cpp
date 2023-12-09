#include "stdafx.h"
#include "Components/include/Cooldown.h"
#include "Managers/include/GameManager.h"
#include "Systems/include/InputHandler.h"
#include "Utilities/include/Helper.h"

void GameManager::Update(float deltaTime)
{
    switch (m_currentGameState)
    {
    case GameState::Loading:
        UpdateLoadingState(deltaTime);
        break;
    }
}

void GameManager::UpdateLoadingState(float deltaTime)
{
    m_timeSpentInLoading += deltaTime;
    if (m_timeSpentInLoading >= m_loadingDuration)
    {
        if (m_previousGameState == GameState::MainMenu)
            m_currentGameState = GameState::Gameplay;
        else if (m_previousGameState == GameState::Gameplay)
            m_currentGameState = GameState::GameOver;
        else if (m_previousGameState == GameState::GameOver)
            m_currentGameState = GameState::MainMenu;
    }
}

void GameManager::HandlePlayButtonClick()
{
    InputHandler& inputHandler = InputHandler::GetInstance();

    if (!inputHandler.GetIsPlayButtonClicked())
        return;
    
    Helper::PlaySoundFromFile(Helper::PATH_TO_BUTTON_CLICK);

    TransitionToLoadingState();
    inputHandler.ResetPlayButtonClick();
}

void GameManager::HandleBackButtonClick()
{
    InputHandler& inputHandler = InputHandler::GetInstance();

    if (!inputHandler.GetIsBackButtonClicked())
        return;

    Helper::PlaySoundFromFile(Helper::PATH_TO_BUTTON_CLICK);

    TransitionToLoadingState();
    inputHandler.ResetBackButtonClick();
}

void GameManager::TransitionToLoadingState()
{
    m_timeSpentInLoading = 0.0f;
 
    // Transitioning from MainMenu to Gameplay
    if (m_currentGameState == GameState::MainMenu)
    {
        Helper::StopSound();
        Helper::PlaySoundFromFile(Helper::PATH_TO_GAMEPLAY_MUSIC);
        Helper::Log("main menu to gameplay, playing sound now before i go into loading");
    }
    
    // Transitioning from Gameplay to GameOver
    else if (m_currentGameState == GameState::Gameplay)
    {
        Helper::StopSound();
        Helper::PlaySoundFromFile(Helper::PATH_TO_NON_GAMEPLAY_MUSIC);
        Helper::Log("gameplay to gameover, playing sound now before i go into loading");
    }

    // Transitioning from GameOver to MainMenu
    else if (m_currentGameState == GameState::GameOver)
    {
        Helper::StopSound();
        Helper::PlaySoundFromFile(Helper::PATH_TO_NON_GAMEPLAY_MUSIC);
        Helper::Log("gameover to gameplay, playing sound now before i go into loading");
    }

    m_previousGameState = m_currentGameState;
    m_currentGameState = GameState::Loading;
}