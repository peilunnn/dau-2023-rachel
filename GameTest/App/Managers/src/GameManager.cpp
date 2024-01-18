#include "stdafx.h"
#include "Managers/include/GameManager.h"
#include "Managers/include/SoundManager.h"
#include "Systems/include/AnimationHandler.h"
#include "Systems/include/HealthHandler.h"
#include "Systems/include/InputHandler.h"
#include "Systems/include/RenderingHandler.h"
#include "Systems/include/ScoreHandler.h"
#include "Systems/include/ShootingHandler.h"
#include "Systems/include/TimerHandler.h"
#include "Utilities/include/Helper.h"
#include "States/include/MainMenuState.h"
using std::make_unique;

GameManager::GameManager() 
{
    m_currentState = make_unique<MainMenuState>();
    m_currentState->Enter();
}

GameManager& GameManager::GetInstance()
{
    static GameManager instance;
    return instance;
}

void GameManager::ChangeState(unique_ptr<IGameState> newState)
{
    if (!m_currentState)
        return;

    m_currentState->Exit();
    m_currentState = move(newState);
    m_currentState->Enter();
}

void GameManager::Update(float deltaTime)
{
    if (!m_currentState)
        return;

    m_currentState->Update(deltaTime);
}

void GameManager::UpdateCrosshairPosition()
{
    EntityManager& entityManager = EntityManager::GetInstance();
    EntityId crosshairEntityId = entityManager.GetCrosshairEntityId();
    Transform* crosshairTransform = entityManager.GetComponent<Transform>(crosshairEntityId);
    float mouseX, mouseY;

    App::GetMousePos(mouseX, mouseY);
    vec3 newPos = vec3(mouseX, mouseY, 0.0f);
    crosshairTransform->SetPosition(newPos);
}

void GameManager::HandlePlayButtonClick()
{
    EntityId playButtonEntityId = EntityManager::GetInstance().GetPlayButtonEntityId();
    HandleButtonClick(playButtonEntityId);
}

void GameManager::HandleBackButtonClick()
{
    EntityId backButtonEntityId = EntityManager::GetInstance().GetBackButtonEntityId();
    HandleButtonClick(backButtonEntityId);
}

void GameManager::HandleQuitButtonClick()
{
    EntityId quitButtonEntityId = EntityManager::GetInstance().GetQuitButtonEntityId();
    HandleButtonClick(quitButtonEntityId);
}

//void GameManager::TransitionToLoadingState()
//{
//    SoundManager& soundManager = SoundManager::GetInstance();
//
//    m_timeSpentInLoading = 0.0f;
// 
//    // Transitioning from MainMenu to Gameplay
//    if ((m_previousGameState == GameState::GameOver || m_previousGameState == GameState::Paused) && m_currentGameState == GameState::MainMenu)
//    {
//        soundManager.StopSound(Helper::PATH_TO_NON_GAMEPLAY_MUSIC);
//        soundManager.PlaySoundFromFile(Helper::PATH_TO_GAMEPLAY_MUSIC, true);
//    }
//    
//    // Transitioning from Gameplay to GameOver
//    else if (m_previousGameState == GameState::MainMenu && m_currentGameState == GameState::Gameplay)
//    {
//        soundManager.StopSound(Helper::PATH_TO_GAMEPLAY_MUSIC);
//        soundManager.PlaySoundFromFile(Helper::PATH_TO_NON_GAMEPLAY_MUSIC, true);
//    }
//
//    // Transitioning from Paused to MainMenu
//    else if (m_previousGameState == GameState::Gameplay && m_currentGameState == GameState::Paused)
//    {
//        soundManager.StopSound(Helper::PATH_TO_GAMEPLAY_MUSIC);
//        soundManager.PlaySoundFromFile(Helper::PATH_TO_NON_GAMEPLAY_MUSIC, true);
//    }
//
//    m_previousGameState = m_currentGameState;
//    m_currentGameState = GameState::Loading;
//}

void GameManager::ResetGame()
{
    m_isFirstEnemyInit = false;
    EntityManager::GetInstance().ResetEnemies();
    SoundManager::GetInstance().ResetSounds();
    HealthHandler::GetInstance().ResetPlayerHealth();
    AnimationHandler::GetInstance().ResetHealthBarAnimation();
    AnimationHandler::GetInstance().ResetPlayerAnimation();
    TimerHandler::GetInstance().ResetTimers();
    ScoreHandler::GetInstance().ResetScore();
    ShootingHandler::GetInstance().ResetBullets();
    RenderingHandler::GetInstance().ShowAllAmmoFilledEntities();
    RenderingHandler::GetInstance().ResetFade();
}

//void GameManager::TogglePause()
//{
//    SoundManager& soundManager = SoundManager::GetInstance();
//
//    if (m_currentGameState == GameState::Paused)
//    {
//        m_currentGameState = m_previousGameState;
//        m_previousGameState = GameState::MainMenu;
//        soundManager.ResumeSound(Helper::PATH_TO_GAMEPLAY_MUSIC);
//    }
//    else
//    {
//        m_previousGameState = m_currentGameState;
//        m_currentGameState = GameState::Paused;
//        soundManager.PauseSound(Helper::PATH_TO_GAMEPLAY_MUSIC);
//    }
//}

//void GameManager::UpdateLoadingState(float deltaTime)
//{
//    m_timeSpentInLoading += deltaTime;
//    if (m_timeSpentInLoading >= m_loadingDuration)
//    {
//        if (m_previousGameState == GameState::MainMenu)
//            m_currentGameState = GameState::Gameplay;
//        else if (m_previousGameState == GameState::Gameplay)
//            m_currentGameState = GameState::GameOver;
//        else if (m_previousGameState == GameState::GameOver || m_previousGameState == GameState::Paused)
//            m_currentGameState = GameState::MainMenu;
//    }
//}

void GameManager::HandleButtonClick(EntityId buttonEntityId)
{
    InputHandler& inputHandler = InputHandler::GetInstance();

    if (!inputHandler.IsButtonClicked(buttonEntityId))
        return;

    SoundManager::GetInstance().PlaySoundFromFile(Helper::PATH_TO_BUTTON_CLICK);
    // TransitionToLoadingState();
}
