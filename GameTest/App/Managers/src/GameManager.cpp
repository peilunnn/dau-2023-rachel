#include "stdafx.h"
#include "Managers/include/GameManager.h"
#include "Managers/include/SoundManager.h"
#include "Systems/include/AnimationHandler.h"
#include "Systems/include/HealthHandler.h"
#include "Systems/include/RenderingHandler.h"
#include "Systems/include/ScoreHandler.h"
#include "Systems/include/ShootingHandler.h"
#include "Systems/include/TimerHandler.h"
#include "Utilities/include/Helper.h"
#include "States/include/MainMenuState.h"
#include "States/include/GameOverState.h"
#include "States/include/GameplayState.h"
using std::make_unique;

GameManager::GameManager()
{
    m_currentGameState = make_unique<MainMenuState>();
    m_previousGameState = make_unique<GameOverState>();
    
    m_currentGameState->Enter();
}

GameManager& GameManager::GetInstance()
{
    static GameManager instance;
    return instance;
}

void GameManager::ChangeState(unique_ptr<IGameState> newState)
{
    if (!m_currentGameState)
        return;

    m_currentGameState->Exit();
    m_previousGameState = std::move(m_currentGameState);
    m_currentGameState = std::move(newState);
    m_currentGameState->Enter();
}

void GameManager::Update(float deltaTime)
{
    if (!m_currentGameState)
        return;

    m_currentGameState->Update(deltaTime);
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