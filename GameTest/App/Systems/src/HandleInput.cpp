#include "stdafx.h"
#include "../include/HandleInput.h"

void HandleInput::Update(EntityManager& entityManager, float deltaTime, Entity playerEntityId) {
    auto velocity = entityManager.GetComponent<Velocity>(playerEntityId);
    if (velocity) {
        float thumbStickX = App::GetController().GetLeftThumbStickX();
        float thumbStickY = App::GetController().GetLeftThumbStickY();

        velocity->velocity.x = (thumbStickX > HandleInput::THUMB_STICK_THRESHOLD || thumbStickX < -HandleInput::THUMB_STICK_THRESHOLD) ? thumbStickX * VELOCITY_MULTIPLIER : 0.0f;
        velocity->velocity.y = (fabs(thumbStickY) > THUMB_STICK_THRESHOLD) ? -thumbStickY * VELOCITY_MULTIPLIER : 0.0f;
    }
}

