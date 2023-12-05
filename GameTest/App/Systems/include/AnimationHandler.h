#pragma once
#include "Utilities/include/SimpleSprite.h"
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"
#include <set>
#include <string>
using namespace std;

class AnimationHandler : public System {
public:
    AnimationHandler() {
        subscribedEvents.insert("BulletHitEnemy");
        subscribedEvents.insert("EnemyHitPlayer");
    }

    void Init(shared_ptr<CSimpleSprite> playerSprite, shared_ptr<CSimpleSprite> enemySprite, shared_ptr<CSimpleSprite> reloadingCircleSprite, shared_ptr<CSimpleSprite> healthBarSprite);
    void Update(EntityManager& entityManager, float deltaTime);
    static void InitEnemyAnimation(shared_ptr<CSimpleSprite> enemySprite);
    void HandleEvent(const Event& event, EntityManager& entityManager, float deltaTime) override;
    void HandleBulletHitEnemy(EntityManager &entityManager, EntityId firstEntityId, EntityId secondEntityId, float deltaTime);
    void HandleEnemyHitPlayer(EntityManager &entityManager, float deltaTime);
    set<string> GetSubscribedEvents() const override {
        return subscribedEvents;
    }

private:
    set<string> subscribedEvents;
    int m_lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_FORWARDS;
    void InitPlayerAnimation(shared_ptr<CSimpleSprite> playerSprite);
    void InitReloadingCircleAnimation(shared_ptr<CSimpleSprite> reloadingCircleSprite);
    void InitHealthBarAnimation(shared_ptr<CSimpleSprite> healthBarSprite);
    void UpdatePlayerAnimation(EntityManager& entityManager, EntityId entityId, float deltaTime);
    void UpdateEnemyAnimation(EntityManager& entityManager, EntityId entityId, float deltaTime);
    void UpdateReloadingCircleAnimation(EntityManager& entityManager, EntityId entityId, float deltaTime);
    void UpdateHealthBarAnimation(EntityManager& entityManager, EntityId entityId, float deltaTime);
};