#include "stdafx.h"
#include "Components/include/Cooldown.h"
#include "Components/include/Renderable.h"
#include "Components/include/Transform.h"
#include "Components/include/Velocity.h"
#include "Managers/include/SystemManager.h"
#include "Systems/include/InputHandler.h"
#include "Systems/include/ShootingHandler.h"
#include "Utilities/include/app.h"
#include "Utilities/include/Helper.h"

InputHandler& InputHandler::GetInstance()
{
    static InputHandler instance;
    return instance;
}

void InputHandler::Update(float deltaTime)
{
    EntityManager &entityManager = EntityManager::GetInstance();
    EntityId playerEntityId = entityManager.GetPlayerEntityId();
    ShootingHandler &shootingHandler = ShootingHandler::GetInstance();
    InputHandler &inputHandler = InputHandler::GetInstance();

    inputHandler.HandlePositionInput(entityManager, playerEntityId, deltaTime);
    inputHandler.HandleShootingInput(entityManager, playerEntityId, deltaTime);
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
    EntityId backButtonEntityId = entityManager.GetPlayButtonEntityId();
    m_isBackButtonClicked = Helper::IsButtonClicked(backButtonEntityId);
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
    Transform *playerTransform = entityManager.GetComponent<Transform>(playerEntityId);

    if (!(playerTransform && App::IsKeyPressed(VK_LBUTTON)))
        return;

    vec3 bulletPos = playerTransform->GetPosition();
    Event playerShootEvent("PlayerShoot", {});
    systemManager.SendEvent(playerShootEvent);
}
