#include "stdafx.h"
#include "../include/MovementHandler.h"
using namespace std;

void MovementHandler::Update(EntityManager &entityManager, float deltaTime)
{
    float screenWidth = ScreenHandler::SCREEN_WIDTH;
    float screenHeight = ScreenHandler::SCREEN_HEIGHT;

    for (auto entity : entityManager.GetEntitiesWithComponents<Tag, Transform, Velocity>())
    {
        auto tag = entityManager.GetComponent<Tag>(entity);
        if (!tag)
            return;

        switch (tag->entityType)
        {
        case EntityType::PLAYER:
            HandlePlayerMovement(entityManager, entity, deltaTime);
            break;
        case EntityType::ENEMY:
            HandleEnemyMovement(entityManager, entity, deltaTime);
            break;
        case EntityType::BULLET:
            HandleBulletMovement(entityManager, entity, deltaTime);
            break;
        }
    }
}

void MovementHandler::HandlePlayerMovement(EntityManager &entityManager, EntityId entityId, float deltaTime)
{
    float topAdjustment = 60.0f;
    float multiplier = 0.25f;
    auto transform = entityManager.GetComponent<Transform>(entityId);
    auto velocity = entityManager.GetComponent<Velocity>(entityId);

    if (!(transform && velocity))
        return;

    float newX = transform->position.x + velocity->velocity.x * deltaTime;
    float newY = transform->position.y + velocity->velocity.y * deltaTime;

    auto sprite = entityManager.GetComponent<Renderable>(entityId)->sprite;
    auto dimensions = Helper::GetSpriteDimensions(sprite, multiplier);

    transform->position.x = std::max(ScreenHandler::SCREEN_LEFT + dimensions.adjustedWidth / 2,
                                     std::min(newX, ScreenHandler::SCREEN_RIGHT - dimensions.adjustedWidth / 2));

    transform->position.y = std::max(ScreenHandler::SCREEN_TOP + dimensions.adjustedHeight / 2 + topAdjustment,
                                     std::min(newY, ScreenHandler::SCREEN_BOTTOM - dimensions.adjustedHeight / 2));
}

void MovementHandler::HandleEnemyMovement(EntityManager& entityManager, EntityId entityId, float deltaTime)
{
    float topAdjustment = 20.0f;
    float bottomAdjustment = -15.0f;
    float multiplier = 0.25f;
    auto transform = entityManager.GetComponent<Transform>(entityId);
    auto velocity = entityManager.GetComponent<Velocity>(entityId);
    auto direction = entityManager.GetComponent<Direction>(entityId);

    if (!(transform && velocity && direction))
        return;

    glm::vec2 movement = velocity->velocity * deltaTime;
    transform->position.x += movement.x;
    transform->position.y += movement.y;

    auto sprite = entityManager.GetComponent<Renderable>(entityId)->sprite;
    auto dimensions = Helper::GetSpriteDimensions(sprite, multiplier);

    if (!(direction->bounced))
    {
        if (transform->position.x <= ScreenHandler::SCREEN_LEFT + dimensions.adjustedWidth / 2 ||
            transform->position.x >= ScreenHandler::SCREEN_RIGHT - dimensions.adjustedWidth / 2)
        {
            velocity->velocity.x *= -1;
            direction->bounced = true;
        }
        if (transform->position.y <= ScreenHandler::SCREEN_TOP + dimensions.adjustedHeight / 2 + topAdjustment ||
            transform->position.y >= ScreenHandler::SCREEN_BOTTOM - dimensions.adjustedHeight / 2 - bottomAdjustment)
        {
            velocity->velocity.y *= -1;
            direction->bounced = true;
        }
    }
    else if (transform->position.x > ScreenHandler::SCREEN_LEFT + dimensions.adjustedWidth / 2 &&
             transform->position.x < ScreenHandler::SCREEN_RIGHT - dimensions.adjustedWidth / 2 &&
             transform->position.y > ScreenHandler::SCREEN_TOP + dimensions.adjustedHeight / 2 &&
             transform->position.y < ScreenHandler::SCREEN_BOTTOM - dimensions.adjustedHeight / 2)
    {
        direction->bounced = false;
    }
}

void MovementHandler::HandleBulletMovement(EntityManager &entityManager, EntityId entityId, float deltaTime)
{
    auto transform = entityManager.GetComponent<Transform>(entityId);
    auto velocity = entityManager.GetComponent<Velocity>(entityId);

    if (!(transform && velocity))
        return;

    glm::vec2 movement = velocity->velocity * deltaTime;
    transform->position.x += movement.x;
    transform->position.y += movement.y;

    if (transform->position.x < ScreenHandler::SCREEN_LEFT || transform->position.x > ScreenHandler::SCREEN_RIGHT ||
        transform->position.y < ScreenHandler::SCREEN_TOP || transform->position.y > ScreenHandler::SCREEN_BOTTOM)
        entityManager.MarkEntityForDeletion(entityId);
}
