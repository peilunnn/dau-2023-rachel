#include "stdafx.h"
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include "../include/HandleInput.h"

void HandleInput::Update(EntityManager& entityManager, float deltaTime, Entity playerEntityId, CSimpleSprite* bulletSprite) {
    HandleInput::HandlePositionInput(entityManager, deltaTime, playerEntityId);
    HandleInput::HandleRotationInput(entityManager, playerEntityId);
    HandleInput::HandleScaleInput(entityManager, playerEntityId);
    HandleInput::HandleShootingInput(entityManager, playerEntityId, bulletSprite);
}

void HandleInput::HandlePositionInput(EntityManager& entityManager, float deltaTime, Entity playerEntityId) {
    auto velocity = entityManager.GetComponent<Velocity>(playerEntityId);
    if (velocity) {
        float thumbStickX = App::GetController().GetLeftThumbStickX();
        float thumbStickY = App::GetController().GetLeftThumbStickY();

        velocity->velocity.x = (thumbStickX > THUMB_STICK_THRESHOLD || thumbStickX < -THUMB_STICK_THRESHOLD) ? thumbStickX * VELOCITY_MULTIPLIER : 0.0f;
        velocity->velocity.y = (fabs(thumbStickY) > THUMB_STICK_THRESHOLD) ? -thumbStickY * VELOCITY_MULTIPLIER : 0.0f;
    }
}

void HandleInput::HandleRotationInput(EntityManager& entityManager, Entity playerEntityId) {
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

void HandleInput::HandleScaleInput(EntityManager& entityManager, Entity playerEntityId) {
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

void HandleInput::HandleShootingInput(EntityManager& entityManager, Entity playerEntityId, CSimpleSprite* bulletSprite)
{
    if (App::IsKeyPressed(VK_LBUTTON)) {
        auto playerTransform = entityManager.GetComponent<Transform>(playerEntityId);
        auto playerVelocity = entityManager.GetComponent<Velocity>(playerEntityId);

        if (playerTransform && playerVelocity) {
            glm::vec3 bulletPos = playerTransform->position;

            // Calculate bullet velocity based on player's velocity direction
            glm::vec2 direction = glm::normalize(playerVelocity->velocity);
            if (glm::length(direction) == 0) {
                direction = glm::vec2(1.0f, 0.0f);
            }
            float bulletSpeed = 1.0f;
            glm::vec2 bulletVelocity = direction * bulletSpeed;

            entityManager.CreateBulletEntity(bulletSprite, bulletPos, bulletVelocity);
        }
    }
}

