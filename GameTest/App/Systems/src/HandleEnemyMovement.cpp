#include "stdafx.h"
#include "../include/HandleEnemyMovement.h"

void HandleEnemyMovement::Update(EntityManager& entityManager, float deltaTime, float screenWidth, float screenHeight)
{
    char debugMsg[256];

    for (auto entity : entityManager.GetEntitiesWithComponents<Transform, Velocity, Direction>()) {
        // TODO: use Tags to distinguish between player and enemy
        // We skip player identity, who will always have id of 0
        if (entity == 0) {
            continue;
        }

        auto transform = entityManager.GetComponent<Transform>(entity);
        auto velocity = entityManager.GetComponent<Velocity>(entity);
        auto direction = entityManager.GetComponent<Direction>(entity);

        if (transform && velocity && direction) {
            sprintf_s(debugMsg, "in HandleMovementUpdate, Enemy Velocity is: X: %f, Y: %f\n", velocity->velocity.x, velocity->velocity.y);
            OutputDebugStringA(debugMsg);

            glm::vec2 movement = velocity->velocity * deltaTime;
            transform->position.x += movement.x * direction->direction.x;
            transform->position.y += movement.y * direction->direction.y;

            if (transform->position.x <= 0 || transform->position.x >= screenWidth) {
                direction->direction.x *= -1;
            }
            if (transform->position.y <= 0 || transform->position.y >= screenHeight) {
                direction->direction.y *= -1;
            }
        }
    }
}
