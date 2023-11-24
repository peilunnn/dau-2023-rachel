#include "stdafx.h"
#include "../include/AnimationHandler.h"
#include <glm/glm.hpp>

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

void AnimationHandler::Update(EntityManager& entityManager, float deltaTime)
{
    for (auto entity : entityManager.GetEntitiesWithComponents<Tag, Animation>())
    {
        auto tag = entityManager.GetComponent<Tag>(entity);

        if (tag->entityType == EntityType::PLAYER)
        {
            UpdatePlayerAnimation(entityManager, entity, deltaTime);
        }

        else if (tag->entityType == EntityType::ENEMY) {
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

        else if (tag->entityType == EntityType::RELOADING_CIRCLE) {
            auto reloadingCircleSprite = entityManager.GetComponent<Renderable>(entity)->sprite;
            auto reloadingCircleAnimation = entityManager.GetComponent<Animation>(entity);
            reloadingCircleAnimation->currentAnimation = RELOADING_CIRCLE_ANIM_SPIN;
            reloadingCircleSprite->SetAnimation(RELOADING_CIRCLE_ANIM_SPIN);
            reloadingCircleSprite->Update(deltaTime);
        }
    }
}

void AnimationHandler::UpdatePlayerAnimation(EntityManager &entityManager, Entity entity, float deltaTime)
{
    auto animation = entityManager.GetComponent<Animation>(entity);
    auto velocity = entityManager.GetComponent<Velocity>(entity);
    auto sprite = entityManager.GetComponent<Renderable>(entity)->sprite;

    if (velocity && animation)
    {
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
        {
            animation->currentAnimation = lastPlayerNonIdleAnimState;
        }
        sprite->SetAnimation(animation->currentAnimation);
        sprite->Update(deltaTime);
    }
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

    Helper::Log("in ProcessBulletHitEnemy, bulletEntity: ", bulletEntity);
    Helper::Log("in ProcessBulletHitEnemy, enemyEntity: ", enemyEntity);

    auto enemyAnimation = entityManager.GetComponent<Animation>(enemyEntity);
    auto enemyVelocity = entityManager.GetComponent<Velocity>(enemyEntity);
    auto enemySprite = entityManager.GetComponent<Renderable>(enemyEntity)->sprite;

    if (!enemyAnimation || !enemyVelocity || !enemySprite)
        return;

    enemyAnimation->currentAnimation = ENEMY_ANIM_MELT;
    enemySprite->SetAnimation(ENEMY_ANIM_MELT);
    enemyVelocity->velocity = glm::vec2(0.0f, 0.0f);

    if (enemySprite->IsAnimationComplete()) {
        enemyAnimation->cooldownTimer = 0.5f;
    }

    entityManager.MarkEntityForDeletion(bulletEntity);
}
