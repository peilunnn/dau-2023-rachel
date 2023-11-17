#include "stdafx.h"
#include <glm/vec3.hpp>
#include "../include/HandleInput.h"


void HandleInput::Update(EntityManager& entityManager, float deltaTime, Entity playerEntityId) {
    auto transform = entityManager.GetComponent<Transform>(playerEntityId);
    auto velocity = entityManager.GetComponent<Velocity>(playerEntityId);

    if (transform && velocity) {
        float thumbStickX = App::GetController().GetLeftThumbStickX();
        float thumbStickY = App::GetController().GetLeftThumbStickY();

        velocity->velocity.x = (thumbStickX > HandleInput::THUMB_STICK_THRESHOLD || thumbStickX < -HandleInput::THUMB_STICK_THRESHOLD) ? thumbStickX * VELOCITY_MULTIPLIER : 0.0f;
        velocity->velocity.y = (fabs(thumbStickY) > THUMB_STICK_THRESHOLD) ? -thumbStickY * VELOCITY_MULTIPLIER : 0.0f;

        // Handle rotation input
        if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false)) {
            transform->rotation.z += ANGLE_DELTA;
        }
        else if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false)) {
            transform->rotation.z -= ANGLE_DELTA;
        }

        // Handle scale input
        if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_UP, false)) {
            transform->scale += glm::vec3(SCALE_DELTA);
        }
        else if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false)) {
            transform->scale -= glm::vec3(SCALE_DELTA);
        }
    }
}

