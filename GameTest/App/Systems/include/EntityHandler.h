#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"

class EntityHandler : public System 
{
public:
    static EntityHandler& GetInstance();
    EntityHandler(EntityHandler const&) = delete;
    void operator=(EntityHandler const&) = delete;

    void HandleEvent(const Event& event, float deltaTime) override;
    void InitEnemy();
    void MoveEntityToRandomPos(EntityId entityId);

    int GetEnemiesToStrike() const { return m_enemiesToStrike; }

private:
    EntityHandler() {
        m_subscribedEvents.insert(EventType::BulletHitEnemy);
        m_subscribedEvents.insert(EventType::EnemyHitPlayer);
        m_subscribedEvents.insert(EventType::BulletOutOfBounds);
        m_subscribedEvents.insert(EventType::PlayerHitAmmoPickup);
        m_subscribedEvents.insert(EventType::PlayerHitHealthPickup);
        m_subscribedEvents.insert(EventType::PlayerHitLightningPickup);
    }

    int m_enemiesToStrike = 0;
    const int m_maxEnemiesToStrike = 5;

    void HandleBulletHitEnemy(EntityManager& entityManager, EntityId bulletEntityId, EntityId enemyEntityId);
    void HandleEnemyHitPlayer(EntityManager& entityManager, EntityId enemyEntityId);
    void HandleBulletOutOfBounds(EntityManager& entityManager, EntityId bulletEntityId);
    void InitLightningStrikes(EntityManager& entityManager);
};
