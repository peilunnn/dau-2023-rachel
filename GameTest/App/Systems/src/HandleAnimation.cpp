#include "stdafx.h"
#include "../include/HandleAnimation.h"
#include <glm/glm.hpp>

void HandleAnimation::Init(CSimpleSprite* playerSprite)
{
    if (playerSprite) {
        float speed = 1.0f / 15.0f;
        playerSprite->CreateAnimation(ANIM_FORWARDS, speed, { 24,25,26,27,28,29,30,31 });
        playerSprite->CreateAnimation(ANIM_BACKWARDS, speed, { 0,1,2,3,4,5,6,7 });
        playerSprite->CreateAnimation(ANIM_LEFT, speed, { 8,9,10,11,12,13,14,15 });
        playerSprite->CreateAnimation(ANIM_RIGHT, speed, { 16,17,18,19,20,21,22,23 });
        playerSprite->CreateAnimation(ANIM_IDLE_BACKWARDS, speed, { 0 });
        playerSprite->CreateAnimation(ANIM_IDLE_LEFT, speed, { 8 });
        playerSprite->CreateAnimation(ANIM_IDLE_RIGHT, speed, { 16 });
        playerSprite->CreateAnimation(ANIM_IDLE_FORWARDS, speed, { 24 });
    }
}

void HandleAnimation::Update(EntityManager& entityManager, float deltaTime) {
    for (auto entity : entityManager.GetEntitiesWithComponents<Animation, Velocity>()) {
        // TODO: use Tags to distinguish between player and enemy
        // We only want player entity of id 0
        if (entity != 0)
        {
            continue;
        }

        auto animation = entityManager.GetComponent<Animation>(entity);
        auto velocity = entityManager.GetComponent<Velocity>(entity);

        if (velocity && animation) {
            if (glm::length(velocity->velocity) > 0) {
                if (velocity->velocity.x > 0) {
                    animation->currentAnimation = ANIM_RIGHT;
                    lastNonIdleAnimState = ANIM_IDLE_RIGHT;
                }
                else if (velocity->velocity.x < 0) {
                    animation->currentAnimation = ANIM_LEFT;
                    lastNonIdleAnimState = ANIM_IDLE_LEFT;
                }
                else if (velocity->velocity.y > 0) {
                    animation->currentAnimation = ANIM_FORWARDS;
                    lastNonIdleAnimState = ANIM_IDLE_FORWARDS;
                }
                else if (velocity->velocity.y < 0) {
                    animation->currentAnimation = ANIM_BACKWARDS;
                    lastNonIdleAnimState = ANIM_IDLE_BACKWARDS;
                }
            }
            else {
                // If not moving, set the animation to the last non-idle state
                animation->currentAnimation = lastNonIdleAnimState;
            }
        }
    }
}