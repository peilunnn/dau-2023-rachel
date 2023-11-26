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

void MovementHandler::HandlePlayerMovement(EntityManager &entityManager, Entity entity, float deltaTime)
{
    float screenWidth = ScreenHandler::SCREEN_WIDTH;
    float screenHeight = ScreenHandler::SCREEN_HEIGHT;
    float borderLeftX = ScreenHandler::BORDER_LEFT_X;
    float borderRightX = ScreenHandler::BORDER_RIGHT_X;
    float borderTopY = ScreenHandler::BORDER_TOP_Y;
    float borderBottomY = ScreenHandler::BORDER_BOTTOM_Y;

    float screenLeft = ScreenHandler::NDCtoScreenX(borderLeftX, ScreenHandler::SCREEN_WIDTH);
    float screenRight = ScreenHandler::NDCtoScreenX(borderRightX, ScreenHandler::SCREEN_WIDTH);
    float screenTop = ScreenHandler::NDCtoScreenY(borderTopY, ScreenHandler::SCREEN_HEIGHT);
    float screenBottom = ScreenHandler::NDCtoScreenY(borderBottomY, ScreenHandler::SCREEN_HEIGHT);
    float topAdjustment = 60.0f;

    float multiplier = 0.25f;
    auto transform = entityManager.GetComponent<Transform>(entity);
    auto velocity = entityManager.GetComponent<Velocity>(entity);

    if (!(transform && velocity))
        return;

    float newX = transform->position.x + velocity->velocity.x * deltaTime;
    float newY = transform->position.y + velocity->velocity.y * deltaTime;

    auto sprite = entityManager.GetComponent<Renderable>(entity)->sprite;
    auto dimensions = Helper::GetSpriteDimensions(sprite, multiplier);

    transform->position.x = std::max(screenLeft + dimensions.adjustedWidth / 2,
        std::min(newX, screenRight - dimensions.adjustedWidth / 2));

    transform->position.y = std::max(screenTop + dimensions.adjustedHeight / 2 + topAdjustment,
        std::min(newY, screenBottom - dimensions.adjustedHeight / 2));
}

void MovementHandler::HandleEnemyMovement(EntityManager &entityManager, Entity entity, float deltaTime)
{
    float screenWidth = ScreenHandler::SCREEN_WIDTH;
    float screenHeight = ScreenHandler::SCREEN_HEIGHT;
    int edgeThreshold = 20;
    auto transform = entityManager.GetComponent<Transform>(entity);
    auto velocity = entityManager.GetComponent<Velocity>(entity);
    auto direction = entityManager.GetComponent<Direction>(entity);

    if (!(transform && velocity && direction))
        return;

        glm::vec2 movement = velocity->velocity * deltaTime;
        transform->position.x += movement.x;
        transform->position.y += movement.y;

    if (!(direction->bounced))
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
        direction->bounced = false;
}

void MovementHandler::HandleBulletMovement(EntityManager &entityManager, Entity entity, float deltaTime)
{
    float screenWidth = ScreenHandler::SCREEN_WIDTH;
    float screenHeight = ScreenHandler::SCREEN_HEIGHT;
    auto transform = entityManager.GetComponent<Transform>(entity);
    auto velocity = entityManager.GetComponent<Velocity>(entity);

    if (!(transform && velocity))
        return;

    glm::vec2 movement = velocity->velocity * deltaTime;
    transform->position.x += movement.x;
    transform->position.y += movement.y;

    if (transform->position.x < 0 || transform->position.x > screenWidth ||
        transform->position.y < 0 || transform->position.y > screenHeight)
        entityManager.MarkEntityForDeletion(entity);
}
