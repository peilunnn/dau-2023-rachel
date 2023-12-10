#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"
#include "Utilities/include/SimpleSprite.h"
#include <set>
#include <string>
using namespace std;

class AnimationHandler : public System
{
public:
    static AnimationHandler& GetInstance();
    AnimationHandler(AnimationHandler const&) = delete;
    void operator=(AnimationHandler const&) = delete;

    void Init();
    void Update(float deltaTime);
    void HandleEvent(const Event &event, float deltaTime) override;
    set<string> GetSubscribedEvents() const override
    {
        return m_subscribedEvents;
    }

    void RotatePlayer(float deltaTime);
    void ResetHealthBarAnimation();
    void ResetPlayerAnimation();

private:
    AnimationHandler() {
        m_subscribedEvents.insert("EnemyHitPlayer");
    }

    set<string> m_subscribedEvents = {};
    int m_lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_FORWARDS;

    void InitPlayerAnimation(EntityManager& entityManager, SpriteManager& spriteManager);
    void InitReloadingCircleAnimation(EntityManager& entityManager, SpriteManager& spriteManager);
    void InitHealthBarAnimation(EntityManager& entityManager, SpriteManager& spriteManager);
    void InitLoadingScreenCharacterAnimation(EntityManager& entityManager, SpriteManager& spriteManager);
    void UpdatePlayerAnimation(EntityManager &entityManager, EntityId entityId, float deltaTime);
    void UpdateReloadingCircleAnimation(EntityManager &entityManager, EntityId entityId, float deltaTime);
    void UpdateHealthBarAnimation(EntityManager& entityManager, EntityId entityId, float deltaTime);
    void UpdateLoadingScreenCharacterAnimation(EntityManager &entityManager, EntityId entityId, float deltaTime);
    void HandleEnemyHitPlayer(EntityManager& entityManager, float deltaTime);
};