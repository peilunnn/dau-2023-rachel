#include "stdafx.h"
#include "../include/HandleMovement.h"
using namespace std;

void HandleMovement::Update(EntityManager& entityManager, float deltaTime, float screenWidth, float screenHeight)
{
    for (auto entity : entityManager.GetEntitiesWithComponents<Transform, Velocity, Tag>()) {
        auto tag = entityManager.GetComponent<Tag>(entity);
        if (tag) {
            switch (tag->entityType) {
            case EntityType::PLAYER:
                HandlePlayerMovement(entityManager, entity, deltaTime, screenWidth, screenHeight);
                break;
            case EntityType::ENEMY:
                HandleEnemyMovement(entityManager, entity, deltaTime, screenWidth, screenHeight);
                break;
            case EntityType::BULLET:
                HandleBulletMovement(entityManager, entity, deltaTime, screenWidth, screenHeight);
                break;
            }
        }
    }
}

void HandleMovement::HandlePlayerMovement(EntityManager& entityManager, Entity entity, float deltaTime, float screenWidth, float screenHeight) {
    float multiplier = 0.25f;

    auto transform = entityManager.GetComponent<Transform>(entity);
    auto velocity = entityManager.GetComponent<Velocity>(entity);

    if (transform && velocity) {
        float newX = transform->position.x + velocity->velocity.x * deltaTime;
        float newY = transform->position.y + velocity->velocity.y * deltaTime;

        auto sprite = dynamic_cast<CSimpleSprite*>(entityManager.GetComponent<Renderable>(entity)->sprite);
        auto dimensions = Helper::GetSpriteDimensions(sprite, multiplier);

        transform->position.x = max(dimensions.adjustedWidth, min(newX, screenWidth - dimensions.adjustedWidth));
        transform->position.y = max(dimensions.adjustedHeight, min(newY, screenHeight - dimensions.adjustedHeight));
    }
}

void HandleMovement::HandleEnemyMovement(EntityManager& entityManager, Entity entity, float deltaTime, float screenWidth, float screenHeight) {
    int multiplier = 0.125f;
    int edgeThreshold = 10;
    auto sprite = dynamic_cast<CSimpleSprite*>(entityManager.GetComponent<Renderable>(entity)->sprite);
    auto dimensions = Helper::GetSpriteDimensions(sprite, multiplier);
    float edgeThresholdX = dimensions.adjustedWidth + edgeThreshold;
    float edgeThresholdY = dimensions.adjustedHeight + edgeThreshold;

    auto transform = entityManager.GetComponent<Transform>(entity);
    auto velocity = entityManager.GetComponent<Velocity>(entity);
    auto direction = entityManager.GetComponent<Direction>(entity);

    if (transform && velocity && direction && sprite) {
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
        else if (transform->position.x > edgeThresholdX && transform->position.x < screenWidth - edgeThresholdX &&
            transform->position.y > edgeThresholdY && transform->position.y < screenHeight - edgeThresholdY) {
            direction->bounced = false;
        }
    }
}

void HandleMovement::HandleBulletMovement(EntityManager& entityManager, Entity entity, float deltaTime, float screenWidth, float screenHeight)
{
    auto transform = entityManager.GetComponent<Transform>(entity);
    auto velocity = entityManager.GetComponent<Velocity>(entity);

    if (transform && velocity) {
        glm::vec2 movement = velocity->velocity * deltaTime;
        transform->position.x += movement.x;
        transform->position.y += movement.y;

        // TODO: add logic to destroy bullet if it's out of bounds
    }
}
