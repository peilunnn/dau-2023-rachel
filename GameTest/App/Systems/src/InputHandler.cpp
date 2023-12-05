#include "stdafx.h"
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include "../include/InputHandler.h"
#include "Components/include/Transform.h"
#include "Components/include/Cooldown.h"
#include "Systems/include/ShootingHandler.h"
#include "Systems/include/MovementHandler.h"
#include "Utilities/include/app.h"

void InputHandler::Update(shared_ptr<CSimpleSprite> bulletSprite, float deltaTime) {
    EntityManager& entityManager = EntityManager::GetInstance();
    EntityId playerEntityId = entityManager.GetPlayerEntityId();
    ShootingHandler& shootingHandler = ShootingHandler::GetInstance();
    InputHandler& inputHandler = InputHandler::GetInstance();

    inputHandler.HandlePositionInput(playerEntityId, deltaTime);
    inputHandler.HandleShootingInput(entityManager, playerEntityId, bulletSprite, deltaTime);
}

void InputHandler::HandlePositionInput(EntityId playerEntityId, float deltaTime) {
    float thumbStickX = App::GetController().GetLeftThumbStickX();
    float thumbStickY = App::GetController().GetLeftThumbStickY();
    vec2 velocity = MovementHandler::GetInstance().GetVelocity(playerEntityId);
    velocity.x = (fabs(thumbStickX) > THUMB_STICK_THRESHOLD) ? thumbStickX * VELOCITY_MULTIPLIER * deltaTime : 0.0f;
    velocity.y = (fabs(thumbStickY) > THUMB_STICK_THRESHOLD) ? -thumbStickY * VELOCITY_MULTIPLIER * deltaTime : 0.0f;
    MovementHandler::GetInstance().SetVelocity(playerEntityId, velocity);
}

void InputHandler::HandleShootingInput(EntityManager& entityManager, EntityId playerEntityId, shared_ptr<CSimpleSprite> bulletSprite, float deltaTime)
{
    float mouseX, mouseY;
    shared_ptr<Transform> playerTransform = entityManager.GetComponent<Transform>(playerEntityId);
    shared_ptr<Cooldown> cooldown = entityManager.GetComponent<Cooldown>(playerEntityId);

    if (!(playerTransform && App::IsKeyPressed(VK_LBUTTON)))
        return;

    vec3 bulletPos = playerTransform->GetPosition();
    App::GetMousePos(mouseX, mouseY);
    ShootingHandler::GetInstance().Shoot(entityManager, playerEntityId, bulletSprite, mouseX, mouseY);
    cooldown->Update(deltaTime);
}


