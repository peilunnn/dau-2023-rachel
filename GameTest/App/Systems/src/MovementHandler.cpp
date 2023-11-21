#include "stdafx.h"
#include "../include/MovementHandler.h"
using namespace std;

void MovementHandler::Update(EntityManager &entityManager, float deltaTime, float screenWidth, float screenHeight)
{
    for (auto entity : entityManager.GetEntitiesWithComponents<Transform, Velocity, Tag>())
    {
        auto tag = entityManager.GetComponent<Tag>(entity);
        if (tag)
        {
            switch (tag->entityType)
            {
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

void MovementHandler::HandlePlayerMovement(EntityManager &entityManager, Entity entity, float deltaTime, float screenWidth, float screenHeight)
{
    float multiplier = 0.25f;
    auto transform = entityManager.GetComponent<Transform>(entity);
    auto velocity = entityManager.GetComponent<Velocity>(entity);

    if (transform && velocity)
    {
        float newX = transform->position.x + velocity->velocity.x * deltaTime;
        float newY = transform->position.y + velocity->velocity.y * deltaTime;

        auto sprite = entityManager.GetComponent<Renderable>(entity)->sprite;
        auto dimensions = Helper::GetSpriteDimensions(sprite, multiplier);

        transform->position.x = max(dimensions.adjustedWidth, min(newX, screenWidth - dimensions.adjustedWidth));
        transform->position.y = max(dimensions.adjustedHeight, min(newY, screenHeight - dimensions.adjustedHeight));
    }
}

void MovementHandler::HandleEnemyMovement(EntityManager &entityManager, Entity entity, float deltaTime, float screenWidth, float screenHeight)
{
    int edgeThreshold = 20;
    auto transform = entityManager.GetComponent<Transform>(entity);
    auto velocity = entityManager.GetComponent<Velocity>(entity);
    auto direction = entityManager.GetComponent<Direction>(entity);

    if (transform && velocity && direction)
    {
        glm::vec2 movement = velocity->velocity * deltaTime;
        transform->position.x += movement.x;
        transform->position.y += movement.y;

        if (!direction->bounced)
        {
            if (transform->position.x <= edgeThreshold || transform->position.x >= screenWidth - edgeThreshold)
            {
                velocity->velocity.x *= -1;
                direction->bounced = true;
            }
            if (transform->position.y <= edgeThreshold || transform->position.y >= screenHeight - edgeThreshold)
            {
                velocity->velocity.y *= -1;
                direction->bounced = true;
            }
        }
        else if (transform->position.x > edgeThreshold && transform->position.x < screenWidth - edgeThreshold &&
                 transform->position.y > edgeThreshold && transform->position.y < screenHeight - edgeThreshold)
        {
            direction->bounced = false;
        }
    }
}

void MovementHandler::HandleBulletMovement(EntityManager &entityManager, Entity entity, float deltaTime, float screenWidth, float screenHeight)
{
    auto transform = entityManager.GetComponent<Transform>(entity);
    auto velocity = entityManager.GetComponent<Velocity>(entity);

    if (transform && velocity)
    {
        glm::vec2 movement = velocity->velocity * deltaTime;
        transform->position.x += movement.x;
        transform->position.y += movement.y;

        // TODO: add logic to destroy bullet if it's out of bounds
        // TODO: handling enemy and bullet movement will have duplicate out of bounds check logic
    }
}