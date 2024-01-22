#include "stdafx.h"
#include "Components/include/Tag.h"
#include "Components/include/Transform.h"
#include "Components/include/Velocity.h"
#include "Managers/include/GameManager.h"
#include "Managers/include/SystemManager.h"
#include "States/include/GameplayState.h"
#include "States/include/PausedState.h"
#include "Systems/include/InputHandler.h"
#include "Systems/include/ShootingHandler.h"
#include "Utilities/include/app.h"
#include "Utilities/include/Helper.h"
using std::make_unique;

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

    HandlePlayerMovementInput(entityManager, playerEntityId, deltaTime);
    HandleShootingInput(entityManager, playerEntityId, deltaTime);
}

bool InputHandler::IsButtonClicked(EntityId entityId)
{
    EntityManager& entityManager = EntityManager::GetInstance();
    Transform* transform = entityManager.GetComponent<Transform>(entityId);
    Renderable* renderable = entityManager.GetComponent<Renderable>(entityId);
    CSimpleSprite* sprite = renderable->GetSprite();
    float scale = sprite->GetScale();
    const float actualWidth = sprite->GetWidth() * scale;
    const float actualHeight = sprite->GetHeight() * scale;
    float left = transform->GetPosition().x - actualWidth / 2.0f;
    float right = transform->GetPosition().x + actualWidth / 2.0f;
    float top = transform->GetPosition().y - actualHeight / 2.0f;
    float bottom = transform->GetPosition().y + actualHeight / 2.0f;
    float mouseX, mouseY;

    App::GetMousePos(mouseX, mouseY);
    bool isWithinX = mouseX >= left && mouseX <= right;
    bool isWithinY = mouseY >= top && mouseY <= bottom;

    return isWithinX && isWithinY && App::IsKeyPressed(VK_LBUTTON);
}

void InputHandler::HandlePauseInput()
{
    GameManager& gameManager = GameManager::GetInstance();
    bool isPKeyPressed = App::IsKeyPressed('P');

    if (isPKeyPressed && !m_wasPPressedLastFrame)
    {
        if (gameManager.GetCurrentGameState()->GetStateEnum() != GameState::Paused)
            gameManager.ChangeState(make_unique<PausedState>());
        else
            gameManager.ChangeState(make_unique<GameplayState>());
    }

    m_wasPPressedLastFrame = isPKeyPressed;
}

void InputHandler::HandlePlayerMovementInput(EntityManager &entityManager, EntityId playerEntityId, float deltaTime)
{
    Tag* playerTag = entityManager.GetComponent<Tag>(playerEntityId);

    if (playerTag->GetEntityState() == EntityState::Dead)
        return;

    Velocity *velocity = entityManager.GetComponent<Velocity>(playerEntityId);
    float thumbStickX = App::GetController().GetLeftThumbStickX();
    float thumbStickY = App::GetController().GetLeftThumbStickY();

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

    ShootingHandler::GetInstance().HandlePlayerShoot();
}
