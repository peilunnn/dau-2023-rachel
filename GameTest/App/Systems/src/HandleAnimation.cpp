#include "stdafx.h"
#include "../include/HandleAnimation.h"
#include <glm/glm.hpp>

void HandleAnimation::InitPlayerAnimation(CSimpleSprite* playerSprite)
{
    if (playerSprite) {
        float speed = 1.0f / 15.0f;
        playerSprite->CreateAnimation(PLAYER_ANIM_FORWARDS, speed, { 24,25,26,27,28,29,30,31 });
        playerSprite->CreateAnimation(PLAYER_ANIM_BACKWARDS, speed, { 0,1,2,3,4,5,6,7 });
        playerSprite->CreateAnimation(PLAYER_ANIM_LEFT, speed, { 8,9,10,11,12,13,14,15 });
        playerSprite->CreateAnimation(PLAYER_ANIM_RIGHT, speed, { 16,17,18,19,20,21,22,23 });
        playerSprite->CreateAnimation(PLAYER_ANIM_IDLE_BACKWARDS, speed, { 0 });
        playerSprite->CreateAnimation(PLAYER_ANIM_IDLE_LEFT, speed, { 8 });
        playerSprite->CreateAnimation(PLAYER_ANIM_IDLE_RIGHT, speed, { 16 });
        playerSprite->CreateAnimation(PLAYER_ANIM_IDLE_FORWARDS, speed, { 24 });
    }
}

void HandleAnimation::InitEnemyAnimation(CSimpleSprite* enemySprite)
{
    if (enemySprite) {
        float speed = 1.0f / 15.0f;
        enemySprite->CreateAnimation(ENEMY_ANIM_IDLE, speed, { 0 });
        enemySprite->CreateAnimation(ENEMY_ANIM_MELT, speed, { 1,2,3,4,5,6,7 });
    }
}

void HandleAnimation::Update(EntityManager& entityManager, float deltaTime) {
    for (auto entity : entityManager.GetEntitiesWithComponents<Animation, Velocity, Tag>()) {
        auto tag = entityManager.GetComponent<Tag>(entity);

        if (tag->entityType == EntityType::PLAYER) {
            UpdatePlayerAnimation(entityManager, entity, deltaTime);
        }
        else if (tag->entityType == EntityType::ENEMY) {
            UpdateEnemyAnimation(entityManager, entity, deltaTime);
        }
    }
}

void HandleAnimation::UpdatePlayerAnimation(EntityManager& entityManager, Entity entity, float deltaTime)
{
    auto animation = entityManager.GetComponent<Animation>(entity);
    auto velocity = entityManager.GetComponent<Velocity>(entity);
    auto sprite = dynamic_cast<CSimpleSprite*>(entityManager.GetComponent<Renderable>(entity)->sprite);

    if (velocity && animation) {
        if (glm::length(velocity->velocity) > 0) {
            if (velocity->velocity.x > 0) {
                animation->currentAnimation = PLAYER_ANIM_RIGHT;
                lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_RIGHT;
            }
            else if (velocity->velocity.x < 0) {
                animation->currentAnimation = PLAYER_ANIM_LEFT;
                lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_LEFT;
            }
            else if (velocity->velocity.y > 0) {
                animation->currentAnimation = PLAYER_ANIM_FORWARDS;
                lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_FORWARDS;
            }
            else if (velocity->velocity.y < 0) {
                animation->currentAnimation = PLAYER_ANIM_BACKWARDS;
                lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_BACKWARDS;
            }
        }
        else {
            // If not moving, set the animation to the last non-idle state
            animation->currentAnimation = lastPlayerNonIdleAnimState;
        }
        sprite->Update(deltaTime);
    }
}

void HandleAnimation::UpdateEnemyAnimation(EntityManager& entityManager, Entity entity, float deltaTime)
{
    auto animation = entityManager.GetComponent<Animation>(entity);

    if (animation) {
        // TODO: implement animation logic
    }
}
