#include "stdafx.h"
#include "../include/HandleMovement.h"

void HandleMovement::Update(EntityManager& entityManager, float deltaTime, float screenWidth, float screenHeight)
{
    HandleMovement::HandlePlayerMovement(entityManager, deltaTime);
    HandleMovement::HandleEnemyMovement(entityManager, deltaTime, screenWidth, screenHeight);
    HandleMovement::HandleBulletMovement(entityManager, deltaTime, screenWidth, screenHeight);
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
        int edgeThreshold = 10;
        auto sprite = dynamic_cast<CSimpleSprite*>(entityManager.GetComponent<Renderable>(entity)->sprite);
        float spriteWidth = sprite->GetWidth();
        float spriteHeight = sprite->GetHeight();
        float spriteWidthAndHeightMultiplier = 0.125f;
        float edgeThresholdX = spriteWidth * spriteWidthAndHeightMultiplier + edgeThreshold;
        float edgeThresholdY = spriteHeight * spriteWidthAndHeightMultiplier + edgeThreshold;

        auto transform = entityManager.GetComponent<Transform>(entity);
        auto velocity = entityManager.GetComponent<Velocity>(entity);
        auto direction = entityManager.GetComponent<Direction>(entity);
        auto tag = entityManager.GetComponent<Tag>(entity);

        if (transform && velocity && direction && tag && sprite) {
            if (tag->entityType != EntityType::ENEMY) {
                continue;
            }

            glm::vec2 movement = velocity->velocity * deltaTime;
            transform->position.x += movement.x;
            transform->position.y += movement.y;

            if (!direction->bounced) {
                if (transform->position.x <= edgeThresholdX || transform->position.x >= screenWidth - edgeThresholdX) {
                    velocity->velocity.x *= -1;
                    direction->bounced = true;
                }
                if (transform->position.y <= edgeThresholdY || transform->position.y >= screenHeight - edgeThresholdY) {
                    velocity->velocity.y *= -1;
                    direction->bounced = true;
                }
            }
            // Reset bounced flag if away from edges
            else if (transform->position.x > edgeThresholdX && transform->position.x < screenWidth - edgeThresholdX &&
                transform->position.y > edgeThresholdY && transform->position.y < screenHeight - edgeThresholdY) {
                direction->bounced = false;
            }
        }
    }
}

void HandleMovement::HandleBulletMovement(EntityManager& entityManager, float deltaTime, float screenWidth, float screenHeight)
{
    for (auto entity : entityManager.GetEntitiesWithComponents<Transform, Velocity, Tag>()) {
        auto transform = entityManager.GetComponent<Transform>(entity);
        auto velocity = entityManager.GetComponent<Velocity>(entity);
        auto tag = entityManager.GetComponent<Tag>(entity);

        if (transform && velocity && tag) {
            if (tag->entityType != EntityType::BULLET) {
                continue;
            }
            
            glm::vec2 movement = velocity->velocity * deltaTime;
            transform->position.x += movement.x;
            transform->position.y += movement.y;

            // TODO: if the bullet is out of screen bounds, we should destroy it
            //if (transform->position.x < 0 || transform->position.x > screenWidth ||
            //    transform->position.y < 0 || transform->position.y > screenHeight) {
            //}
        }
    }
}