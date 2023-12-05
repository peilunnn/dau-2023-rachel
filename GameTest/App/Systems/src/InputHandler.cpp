#include "stdafx.h"
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include "../include/InputHandler.h"
#include "Components/include/Transform.h"
#include "Systems/include/ShootingHandler.h"
#include "Systems/include/MovementHandler.h"
#include "Utilities/include/app.h"

void InputHandler::Update(float deltaTime, EntityId playerEntityId, shared_ptr<CSimpleSprite> bulletSprite) {
    EntityManager& entityManager = EntityManager::GetInstance();
    ShootingHandler& shootingHandler = ShootingHandler::GetInstance();
    InputHandler& inputHandler = InputHandler::GetInstance();
    float newTimeSinceLastShot = shootingHandler.GetTimeSinceLastShot() + deltaTime;

    inputHandler.HandlePositionInput(deltaTime, playerEntityId);
    shootingHandler.SetTimeSinceLastShot(newTimeSinceLastShot);
    inputHandler.HandleShootingInput(entityManager, playerEntityId, bulletSprite);
}

void InputHandler::HandlePositionInput(float deltaTime, EntityId playerEntityId) {
    float thumbStickX = App::GetController().GetLeftThumbStickX();
    float thumbStickY = App::GetController().GetLeftThumbStickY();
    vec2 velocity = MovementHandler::GetInstance().GetVelocity(playerEntityId);
    velocity.x = (fabs(thumbStickX) > THUMB_STICK_THRESHOLD) ? thumbStickX * VELOCITY_MULTIPLIER * deltaTime : 0.0f;
    velocity.y = (fabs(thumbStickY) > THUMB_STICK_THRESHOLD) ? -thumbStickY * VELOCITY_MULTIPLIER * deltaTime : 0.0f;
    MovementHandler::GetInstance().SetVelocity(playerEntityId, velocity);
}

void InputHandler::HandleShootingInput(EntityManager& entityManager, EntityId playerEntityId, shared_ptr<CSimpleSprite> bulletSprite)
{
    float mouseX, mouseY;
    shared_ptr<Transform> playerTransform = entityManager.GetComponent<Transform>(playerEntityId);

    if (!(playerTransform && App::IsKeyPressed(VK_LBUTTON)))
        return;

    vec3 bulletPos = playerTransform->GetPosition();
    App::GetMousePos(mouseX, mouseY);
    ShootingHandler::GetInstance().Shoot(entityManager, playerEntityId, bulletSprite, mouseX, mouseY);
}


