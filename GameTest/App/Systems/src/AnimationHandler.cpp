#include "stdafx.h"
#include "../include/AnimationHandler.h"
#include <glm/glm.hpp>

void AnimationHandler::Init(std::shared_ptr<CSimpleSprite> playerSprite, std::shared_ptr<CSimpleSprite> enemySprite, std::shared_ptr<CSimpleSprite> reloadingCircleSprite, std::shared_ptr<CSimpleSprite> healthBarSprite)
{
    InitPlayerAnimation(playerSprite);
    InitEnemyAnimation(enemySprite);
    InitReloadingCircleAnimation(reloadingCircleSprite);
    InitHealthBarAnimation(healthBarSprite);
}

void AnimationHandler::InitPlayerAnimation(std::shared_ptr<CSimpleSprite> playerSprite)
{
    if (!playerSprite)
        return;

    float speed = 1.0f / 15.0f;
    playerSprite->CreateAnimation(PLAYER_ANIM_FORWARDS, speed, {24, 25, 26, 27, 28, 29, 30, 31});
    playerSprite->CreateAnimation(PLAYER_ANIM_BACKWARDS, speed, {0, 1, 2, 3, 4, 5, 6, 7});
    playerSprite->CreateAnimation(PLAYER_ANIM_LEFT, speed, {8, 9, 10, 11, 12, 13, 14, 15});
    playerSprite->CreateAnimation(PLAYER_ANIM_RIGHT, speed, {16, 17, 18, 19, 20, 21, 22, 23});
    playerSprite->CreateAnimation(PLAYER_ANIM_IDLE_BACKWARDS, speed, {0});
    playerSprite->CreateAnimation(PLAYER_ANIM_IDLE_LEFT, speed, {8});
    playerSprite->CreateAnimation(PLAYER_ANIM_IDLE_RIGHT, speed, {16});
    playerSprite->CreateAnimation(PLAYER_ANIM_IDLE_FORWARDS, speed, {24});
}

void AnimationHandler::InitEnemyAnimation(std::shared_ptr<CSimpleSprite> enemySprite)
{
    if (!enemySprite)
        return;

    float speed = 1.0f / 15.0f;
    enemySprite->CreateAnimation(ENEMY_ANIM_IDLE, speed, {0});
    enemySprite->CreateAnimation(ENEMY_ANIM_MELT, speed, {1, 2, 3, 4, 5, 6, 7});
}

void AnimationHandler::InitReloadingCircleAnimation(std::shared_ptr<CSimpleSprite> reloadingCircleSprite)
{
    if (!reloadingCircleSprite)
        return;

    float speed = 1.0f / 15.0f;
    reloadingCircleSprite->CreateAnimation(RELOADING_CIRCLE_ANIM_SPIN, speed, { 1, 2, 3, 4, 5, 6 });
}

void AnimationHandler::InitHealthBarAnimation(std::shared_ptr<CSimpleSprite> healthBarSprite)
{
    if (!healthBarSprite)
        return;

    float speed = 1.0f / 15.0f;
    healthBarSprite->CreateAnimation(HEALTH_100, speed, { 0 });
    healthBarSprite->CreateAnimation(HEALTH_80, speed, { 2 });
    healthBarSprite->CreateAnimation(HEALTH_60, speed, { 4 });
    healthBarSprite->CreateAnimation(HEALTH_40, speed, { 1 });
    healthBarSprite->CreateAnimation(HEALTH_20, speed, { 3 });
    healthBarSprite->CreateAnimation(HEALTH_0, speed, { 5 });
}

void AnimationHandler::Update(EntityManager& entityManager, float deltaTime)
{
    for (auto entity : entityManager.GetEntitiesWithComponents<Tag, Animation>())
    {
        auto tag = entityManager.GetComponent<Tag>(entity);

        switch (tag->entityType)
        {
        case EntityType::PLAYER:
            UpdatePlayerAnimation(entityManager, entity, deltaTime);
            break;
        case EntityType::ENEMY:
            UpdateEnemyAnimation(entityManager, entity, deltaTime);
            break;
        case EntityType::RELOADING_CIRCLE:
            UpdateReloadingCircleAnimation(entityManager, entity, deltaTime);
            break;
        case EntityType::HEALTH_BAR:
            UpdateHealthBarAnimation(entityManager, entity, deltaTime);
            break;
        }
    }
}

void AnimationHandler::UpdatePlayerAnimation(EntityManager &entityManager, Entity entity, float deltaTime)
{
    auto animation = entityManager.GetComponent<Animation>(entity);
    auto velocity = entityManager.GetComponent<Velocity>(entity);
    auto sprite = entityManager.GetComponent<Renderable>(entity)->sprite;
    auto tag = entityManager.GetComponent<Tag>(entity);

    if (!(velocity && animation))
        return;

    if (glm::length(velocity->velocity) > 0)
    {
        if (velocity->velocity.x > 0)
        {
            animation->currentAnimation = PLAYER_ANIM_RIGHT;
            lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_RIGHT;
        }
        else if (velocity->velocity.x < 0)
        {
            animation->currentAnimation = PLAYER_ANIM_LEFT;
            lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_LEFT;
        }
        else if (velocity->velocity.y > 0)
        {
            animation->currentAnimation = PLAYER_ANIM_FORWARDS;
            lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_FORWARDS;
        }
        else if (velocity->velocity.y < 0)
        {
            animation->currentAnimation = PLAYER_ANIM_BACKWARDS;
            lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_BACKWARDS;
        }
    }
    else
        animation->currentAnimation = lastPlayerNonIdleAnimState;

    sprite->SetAnimation(animation->currentAnimation);
    sprite->Update(deltaTime);
}

void AnimationHandler::UpdateEnemyAnimation(EntityManager& entityManager, Entity entity, float deltaTime)
{
    auto enemySprite = entityManager.GetComponent<Renderable>(entity)->sprite;
    auto enemyAnimation = entityManager.GetComponent<Animation>(entity);
    enemySprite->Update(deltaTime);

    if (enemyAnimation->cooldownTimer > 0.0f) {
        enemyAnimation->cooldownTimer -= deltaTime;
        if (enemyAnimation->cooldownTimer <= 0.0f) {
            entityManager.MarkEntityForDeletion(entity);
        }
    }
}

void AnimationHandler::UpdateReloadingCircleAnimation(EntityManager& entityManager, Entity entity, float deltaTime)
{
    auto reloadingCircleSprite = entityManager.GetComponent<Renderable>(entity)->sprite;
    auto reloadingCircleAnimation = entityManager.GetComponent<Animation>(entity);
    reloadingCircleAnimation->currentAnimation = RELOADING_CIRCLE_ANIM_SPIN;
    reloadingCircleSprite->SetAnimation(RELOADING_CIRCLE_ANIM_SPIN);
    reloadingCircleSprite->Update(deltaTime);
}

void AnimationHandler::UpdateHealthBarAnimation(EntityManager& entityManager, Entity entity, float deltaTime)
{
    auto healthBarSprite = entityManager.GetComponent<Renderable>(entity)->sprite;
    healthBarSprite->Update(deltaTime);
}

void AnimationHandler::ProcessBulletHitEnemy(EntityManager &entityManager, Entity entity1, Entity entity2, float deltaTime)
{
    Entity bulletEntity, enemyEntity;

    auto tag1 = entityManager.GetComponent<Tag>(entity1);
    auto tag2 = entityManager.GetComponent<Tag>(entity2);

    if (!tag1 || !tag2)
        return;

    if (tag1->entityType == EntityType::BULLET) {
        bulletEntity = entity1;
        enemyEntity = entity2;
    }
    else {
        bulletEntity = entity2;
        enemyEntity = entity1;
    }

    auto enemyAnimation = entityManager.GetComponent<Animation>(enemyEntity);
    auto enemyVelocity = entityManager.GetComponent<Velocity>(enemyEntity);
    auto enemySprite = entityManager.GetComponent<Renderable>(enemyEntity)->sprite;

    if (!enemyAnimation || !enemyVelocity || !enemySprite)
        return;

    enemyAnimation->currentAnimation = ENEMY_ANIM_MELT;
    enemySprite->SetAnimation(ENEMY_ANIM_MELT);
    enemyVelocity->velocity = glm::vec2(0.0f, 0.0f);

    if (enemySprite->IsAnimationComplete())
        enemyAnimation->cooldownTimer = 0.3f;

    entityManager.MarkEntityForDeletion(bulletEntity);
}

void AnimationHandler::ProcessEnemyHitPlayer(EntityManager& entityManager, float deltaTime)
{
    Entity playerEntity = entityManager.GetPlayerEntity();
    Entity healthBarEntity = entityManager.GetHealthBarEntity();
    auto health = entityManager.GetComponent<Health>(playerEntity);
    auto animation = entityManager.GetComponent<Animation>(healthBarEntity);
    auto healthBarSprite = entityManager.GetComponent<Renderable>(healthBarEntity)->sprite;

    if (!health || !healthBarSprite)
        return;

    int frameIndex = static_cast<int>((100 - health->currentHealth) / 20);
    frameIndex = std::min(frameIndex, 5);
    animation->currentAnimation = frameIndex;
    healthBarSprite->SetAnimation(frameIndex);
}
