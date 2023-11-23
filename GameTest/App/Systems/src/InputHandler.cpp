#include "stdafx.h"
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include "../include/InputHandler.h"

void InputHandler::Update(EntityManager& entityManager, float deltaTime, Entity playerEntityId, std::shared_ptr<CSimpleSprite> bulletSprite) {
    InputHandler::HandlePositionInput(entityManager, deltaTime, playerEntityId);
    InputHandler::HandleRotationInput(entityManager, playerEntityId);
    InputHandler::HandleScaleInput(entityManager, playerEntityId);
    InputHandler::HandleShootingInput(entityManager, playerEntityId, bulletSprite);
}

void InputHandler::HandlePositionInput(EntityManager& entityManager, float deltaTime, Entity playerEntityId) {
    auto velocity = entityManager.GetComponent<Velocity>(playerEntityId);
    if (velocity) {
        float thumbStickX = App::GetController().GetLeftThumbStickX();
        float thumbStickY = App::GetController().GetLeftThumbStickY();

        velocity->velocity.x = (thumbStickX > THUMB_STICK_THRESHOLD || thumbStickX < -THUMB_STICK_THRESHOLD) ? thumbStickX * VELOCITY_MULTIPLIER : 0.0f;
        velocity->velocity.y = (fabs(thumbStickY) > THUMB_STICK_THRESHOLD) ? -thumbStickY * VELOCITY_MULTIPLIER : 0.0f;
    }
}

void InputHandler::HandleRotationInput(EntityManager& entityManager, Entity playerEntityId) {
    auto transform = entityManager.GetComponent<Transform>(playerEntityId);
    if (transform) {
        if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false)) {
            transform->rotation.z += ANGLE_DELTA;
        }
        else if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false)) {
            transform->rotation.z -= ANGLE_DELTA;
        }
    }
}

void InputHandler::HandleScaleInput(EntityManager& entityManager, Entity playerEntityId) {
    auto transform = entityManager.GetComponent<Transform>(playerEntityId);
    if (transform) {
        if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_UP, false)) {
            transform->scale += glm::vec3(SCALE_DELTA);
        }
        else if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false)) {
            transform->scale -= glm::vec3(SCALE_DELTA);
        }
    }
}

void InputHandler::HandleShootingInput(EntityManager& entityManager, Entity playerEntityId, std::shared_ptr<CSimpleSprite> bulletSprite)
{
    if (App::IsKeyPressed(VK_LBUTTON)) {
        auto playerTransform = entityManager.GetComponent<Transform>(playerEntityId);

        if (playerTransform) {
            glm::vec3 bulletPos = playerTransform->position;

            float mouseX, mouseY;
            App::GetMousePos(mouseX, mouseY);

            ShootingHandler::Shoot(entityManager, playerEntityId, bulletSprite, mouseX, mouseY);
        }
    }
}


