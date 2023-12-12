#include "stdafx.h"
#include "Components/include/Transform.h"
#include "Components/include/Velocity.h"
#include "Managers/include/GameManager.h"
#include "Managers/include/SystemManager.h"
#include "Systems/include/InputHandler.h"
#include "Utilities/include/app.h"
#include "Utilities/include/Helper.h"

InputHandler& InputHandler::GetInstance()
{
    static InputHandler instance;
    return instance;
}

void InputHandler::Update(float deltaTime)
{
    GameManager& gameManager = GameManager::GetInstance();
    EntityManager& entityManager = EntityManager::GetInstance();
    EntityId playerEntityId = entityManager.GetPlayerEntityId();

    if (gameManager.GetCurrentGameState() == GameState::Gameplay || gameManager.GetCurrentGameState() == GameState::Paused)
        HandlePauseInput();
    
    if (gameManager.GetCurrentGameState() == GameState::Paused)
        return;

    HandlePositionInput(entityManager, playerEntityId, deltaTime);
    HandleShootingInput(entityManager, playerEntityId, deltaTime);
}

void InputHandler::SetIsPlayButtonClicked()
{
    EntityManager& entityManager = EntityManager::GetInstance();
    EntityId playButtonEntityId = entityManager.GetPlayButtonEntityId();
    m_isPlayButtonClicked = Helper::IsButtonClicked(playButtonEntityId);
}

void InputHandler::SetIsBackButtonClicked()
{
    EntityManager& entityManager = EntityManager::GetInstance();
    EntityId backButtonEntityId = entityManager.GetBackButtonEntityId();
    m_isBackButtonClicked = Helper::IsButtonClicked(backButtonEntityId);
}

void InputHandler::SetIsQuitButtonClicked()
{
    EntityManager& entityManager = EntityManager::GetInstance();
    EntityId quitButtonEntityId = entityManager.GetQuitButtonEntityId();
    m_isQuitButtonClicked = Helper::IsButtonClicked(quitButtonEntityId);
}

void InputHandler::HandlePositionInput(EntityManager &entityManager, EntityId playerEntityId, float deltaTime)
{
    float thumbStickX = App::GetController().GetLeftThumbStickX();
    float thumbStickY = App::GetController().GetLeftThumbStickY();

    Velocity *velocity = entityManager.GetComponent<Velocity>(playerEntityId);
    vec2 currentVelocity = velocity->GetVelocity();
    currentVelocity.x = (fabs(thumbStickX) > THUMB_STICK_THRESHOLD) ? thumbStickX * VELOCITY_MULTIPLIER * deltaTime : 0.0f;
    currentVelocity.y = (fabs(thumbStickY) > THUMB_STICK_THRESHOLD) ? -thumbStickY * VELOCITY_MULTIPLIER * deltaTime : 0.0f;
    velocity->SetVelocity(currentVelocity);
}

void InputHandler::HandleShootingInput(EntityManager &entityManager, EntityId playerEntityId, float deltaTime)
{
    SystemManager &systemManager = SystemManager::GetInstance();
    Tag* playerTag = entityManager.GetComponent<Tag>(playerEntityId);
    Transform *playerTransform = entityManager.GetComponent<Transform>(playerEntityId);

    if (playerTag->GetEntityState() == EntityState::Dead || !(App::IsKeyPressed(VK_LBUTTON)))
        return;

    vec3 bulletPos = playerTransform->GetPosition();
    Event playerShootEvent("PlayerShoot", {});
    systemManager.SendEvent(playerShootEvent);
}

void InputHandler::HandlePauseInput()
{
    bool isPKeyPressed = App::IsKeyPressed('P');

    if (isPKeyPressed && !m_wasPPressedLastFrame)
        GameManager::GetInstance().TogglePause();

    m_wasPPressedLastFrame = isPKeyPressed;
}
