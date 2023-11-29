#include "stdafx.h"
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include "../include/InputHandler.h"
#include "Components/include/Transform.h"
#include "Components/include/Velocity.h"
#include "Systems/include/ShootingHandler.h"
#include "Utilities/include/app.h"

void InputHandler::Update(EntityManager& entityManager, float deltaTime, EntityId playerEntity, shared_ptr<CSimpleSprite> bulletSprite) {
    InputHandler::HandlePositionInput(entityManager, deltaTime, playerEntity);
 
    float newTimeSinceLastShot = ShootingHandler::GetTimeSinceLastShot() + deltaTime;
    ShootingHandler::SetTimeSinceLastShot(newTimeSinceLastShot);
    InputHandler::HandleShootingInput(entityManager, playerEntity, bulletSprite);
}

void InputHandler::HandlePositionInput(EntityManager& entityManager, float deltaTime, EntityId playerEntity) {
    auto velocity = entityManager.GetComponent<Velocity>(playerEntity);

    if (!velocity)
        return;

    float thumbStickX = App::GetController().GetLeftThumbStickX();
    float thumbStickY = App::GetController().GetLeftThumbStickY();

    velocity->velocity.x = (fabs(thumbStickX) > THUMB_STICK_THRESHOLD) ? thumbStickX * VELOCITY_MULTIPLIER * deltaTime : 0.0f;
    velocity->velocity.y = (fabs(thumbStickY) > THUMB_STICK_THRESHOLD) ? -thumbStickY * VELOCITY_MULTIPLIER * deltaTime : 0.0f;
}

void InputHandler::HandleShootingInput(EntityManager& entityManager, EntityId playerEntity, shared_ptr<CSimpleSprite> bulletSprite)
{
    float mouseX, mouseY;
    auto playerTransform = entityManager.GetComponent<Transform>(playerEntity);

    if (!(playerTransform && App::IsKeyPressed(VK_LBUTTON)))
        return;

    glm::vec3 bulletPos = playerTransform->position;
    App::GetMousePos(mouseX, mouseY);
    ShootingHandler::Shoot(entityManager, playerEntity, bulletSprite, mouseX, mouseY);
}


