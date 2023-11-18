#include "stdafx.h"
#include "../include/HandleMovement.h"

void HandleMovement::HandlePlayerMovement(EntityManager& entityManager, float deltaTime) {
    for (auto entity : entityManager.GetEntitiesWithComponents<Transform, Velocity>()) {
        auto transform = entityManager.GetComponent<Transform>(entity);
        auto velocity = entityManager.GetComponent<Velocity>(entity);

        if (transform && velocity) {
            transform->position.x += velocity->velocity.x * deltaTime;
            transform->position.y += velocity->velocity.y * deltaTime;
        }
    }
}

void HandleMovement::HandleEnemyMovement(EntityManager& entityManager, float deltaTime, float screenWidth, float screenHeight) {
    for (auto entity : entityManager.GetEntitiesWithComponents<Transform, Velocity, Direction>()) {
        // TODO: use Tags to distinguish between player and enemy
        // We skip player entity, who will always have id of 0
        if (entity == 0) {
            continue;
        }

        auto transform = entityManager.GetComponent<Transform>(entity);
        auto velocity = entityManager.GetComponent<Velocity>(entity);
        auto direction = entityManager.GetComponent<Direction>(entity);
        int edgeThreshold = 10;

        if (transform && velocity && direction) {
            glm::vec2 movement = velocity->velocity * deltaTime;
            transform->position.x += movement.x;
            transform->position.y += movement.y;

            if (!direction->bounced) {
                if (transform->position.x <= 0 || transform->position.x >= screenWidth) {
                    velocity->velocity.x *= -1;
                    direction->bounced = true;
                }
                if (transform->position.y <= 0 || transform->position.y >= screenHeight) {
                    velocity->velocity.y *= -1;
                    direction->bounced = true;
                }
            }
            // Reset bounced flag if away from edges
            else if (transform->position.x > edgeThreshold && transform->position.x < screenWidth - edgeThreshold &&
                transform->position.y > edgeThreshold && transform->position.y < screenHeight - edgeThreshold) {
                direction->bounced = false;
            }
        }
    }
}
