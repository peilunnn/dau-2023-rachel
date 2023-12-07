#include "stdafx.h"
#include "Components/include/Cooldown.h"
#include "Components/include/Renderable.h"
#include "Components/include/Transform.h"
#include "Components/include/Velocity.h"
#include "Systems/include/InputHandler.h"
#include "Systems/include/ShootingHandler.h"
#include "Utilities/include/app.h"
#include "Utilities/include/Helper.h"

void InputHandler::Update(float deltaTime)
{
    EntityManager &entityManager = EntityManager::GetInstance();
    EntityId playerEntityId = entityManager.GetPlayerEntityId();
    ShootingHandler &shootingHandler = ShootingHandler::GetInstance();
    InputHandler &inputHandler = InputHandler::GetInstance();

    inputHandler.HandlePositionInput(entityManager, playerEntityId, deltaTime);
    inputHandler.HandleShootingInput(entityManager, playerEntityId, deltaTime);
}

void InputHandler::SetIsPlayButtonClicked() {
    EntityManager& entityManager = EntityManager::GetInstance();
    EntityId playButtonEntityId = entityManager.GetPlayButtonEntityId();
    Transform* transform = entityManager.GetComponent<Transform>(playButtonEntityId);
    Renderable* renderable = entityManager.GetComponent<Renderable>(playButtonEntityId);
    CSimpleSprite* sprite = renderable->GetSprite();
    const float widthBuffer = sprite->GetWidth() / 6.0f;
    const float heightBuffer = sprite->GetHeight() / 6.0f;

    float mouseX, mouseY;
    App::GetMousePos(mouseX, mouseY);

    float left = transform->GetPosition().x - widthBuffer;
    float right = transform->GetPosition().x + widthBuffer;
    float top = transform->GetPosition().y - heightBuffer;
    float bottom = transform->GetPosition().y + heightBuffer;

    bool isWithinX = mouseX >= left && mouseX <= right;
    bool isWithinY = mouseY >= top && mouseY <= bottom;

    m_isPlayButtonClicked = isWithinX && isWithinY && App::IsKeyPressed(VK_LBUTTON);
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
    float mouseX, mouseY;
    Transform *playerTransform = entityManager.GetComponent<Transform>(playerEntityId);
    Cooldown *cooldown = entityManager.GetComponent<Cooldown>(playerEntityId);

    if (!(playerTransform && App::IsKeyPressed(VK_LBUTTON)))
        return;

    vec3 bulletPos = playerTransform->GetPosition();
    App::GetMousePos(mouseX, mouseY);
    ShootingHandler::GetInstance().Shoot(entityManager, playerEntityId, mouseX, mouseY);
    cooldown->Update(deltaTime);
}
