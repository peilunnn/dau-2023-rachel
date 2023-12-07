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
    static AnimationHandler& GetInstance() {
        static AnimationHandler instance;
        return instance;
    }
    AnimationHandler(AnimationHandler const&) = delete;
    void operator=(AnimationHandler const&) = delete;

    void Init(CSimpleSprite* playerSprite, CSimpleSprite* enemySprite, CSimpleSprite* reloadingCircleSprite, CSimpleSprite* healthBarSprite);
    void Update(float deltaTime);
    void HandleEvent(const Event &event, float deltaTime) override;
    set<string> GetSubscribedEvents() const override
    {
        return m_subscribedEvents;
    }

private:
    AnimationHandler() {
        m_subscribedEvents.insert("EnemyHitPlayer");
    }

    set<string> m_subscribedEvents;
    int m_lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_FORWARDS;

    void InitPlayerAnimation(CSimpleSprite* playerSprite);
    void InitReloadingCircleAnimation(CSimpleSprite* reloadingCircleSprite);
    void InitHealthBarAnimation(CSimpleSprite* healthBarSprite);
    void UpdatePlayerAnimation(EntityManager &entityManager, EntityId entityId, float deltaTime);
    void UpdateReloadingCircleAnimation(EntityManager &entityManager, EntityId entityId, float deltaTime);
    void UpdateHealthBarAnimation(EntityManager &entityManager, EntityId entityId, float deltaTime);
    void HandleEnemyHitPlayer(EntityManager &entityManager, float deltaTime);
};