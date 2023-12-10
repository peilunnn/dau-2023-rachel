#include "stdafx.h"
#include "Managers/include/GameManager.h"
#include "Managers/include/SoundManager.h"
#include "Systems/include/AnimationHandler.h"
#include "Systems/include/HealthHandler.h"
#include "Systems/include/InputHandler.h"
#include "Systems/include/TimerHandler.h"
#include "Systems/include/ScoreHandler.h"
#include "Utilities/include/Helper.h"

GameManager& GameManager::GetInstance()
{
    static GameManager instance;
    return instance;
}

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

    SoundManager::GetInstance().PlaySoundFromFile(Helper::PATH_TO_BUTTON_CLICK);

    TransitionToLoadingState();
    inputHandler.ResetPlayButtonClick();
}

void GameManager::HandleBackButtonClick()
{
    InputHandler& inputHandler = InputHandler::GetInstance();

    if (!inputHandler.GetIsBackButtonClicked())
        return;

    SoundManager::GetInstance().PlaySoundFromFile(Helper::PATH_TO_BUTTON_CLICK);

    TransitionToLoadingState();
    inputHandler.ResetBackButtonClick();
}

void GameManager::TransitionToLoadingState()
{
    SoundManager& soundManager = SoundManager::GetInstance();

    m_timeSpentInLoading = 0.0f;
 
    // Transitioning from MainMenu to Gameplay
    if (m_previousGameState == GameState::GameOver && m_currentGameState == GameState::MainMenu)
    {
        soundManager.StopSound(Helper::PATH_TO_NON_GAMEPLAY_MUSIC);
        soundManager.PlaySoundFromFile(Helper::PATH_TO_GAMEPLAY_MUSIC, true);
    }
    
    // Transitioning from Gameplay to GameOver
    else if (m_previousGameState == GameState::MainMenu && m_currentGameState == GameState::Gameplay)
    {
        soundManager.StopSound(Helper::PATH_TO_GAMEPLAY_MUSIC);
        soundManager.PlaySoundFromFile(Helper::PATH_TO_NON_GAMEPLAY_MUSIC, true);
    }

    m_previousGameState = m_currentGameState;
    m_currentGameState = GameState::Loading;
}

void GameManager::ResetGame()
{
    HealthHandler::GetInstance().ResetPlayerHealth();
    AnimationHandler::GetInstance().ResetHealthBarAnimation();
    AnimationHandler::GetInstance().ResetPlayerAnimation();
    TimerHandler::GetInstance().ResetTimers();
    ScoreHandler::GetInstance().ResetScore();
}
