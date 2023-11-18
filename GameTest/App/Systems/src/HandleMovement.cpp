#include "stdafx.h"
#include "../include/HandleMovement.h"

void HandleMovement::Update(EntityManager& entityManager, float deltaTime, float screenWidth, float screenHeight)
{
    HandleMovement::HandlePlayerMovement(entityManager, deltaTime);
    HandleMovement::HandleEnemyMovement(entityManager, deltaTime, screenWidth, screenHeight);
}

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
    for (auto entity : entityManager.GetEntitiesWithComponents<Transform, Velocity, Direction, Tag>()) {
        auto transform = entityManager.GetComponent<Transform>(entity);
        auto velocity = entityManager.GetComponent<Velocity>(entity);
        auto direction = entityManager.GetComponent<Direction>(entity);
        auto tag = entityManager.GetComponent<Tag>(entity);
        int edgeThreshold = 10;

        if (transform && velocity && direction && tag) {
            if (tag->entityType != EntityType::ENEMY) {
                continue;
            }

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
            else if (transform->position.x > edgeThreshold && transform->position.x < screenWidth - edgeThreshold &&
                transform->position.y > edgeThreshold && transform->position.y < screenHeight - edgeThreshold) {
                direction->bounced = false;
            }
        }
    }
}