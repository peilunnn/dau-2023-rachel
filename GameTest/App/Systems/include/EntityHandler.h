#pragma once
#include "Systems/include/System.h"
#include <set>

class EntityHandler : public System {
public:
    static EntityHandler& GetInstance();
    EntityHandler(EntityHandler const&) = delete;
    void operator=(EntityHandler const&) = delete;

    void HandleEvent(const Event& event, float deltaTime) override;
    void InitEnemy();
    void MoveEntityToRandomPos(EntityId entityId);

private:
    EntityHandler() {
        m_subscribedEvents.insert(EventType::BulletHitEnemy);
        m_subscribedEvents.insert(EventType::EnemyHitPlayer);
        m_subscribedEvents.insert(EventType::BulletOutOfBounds);
        m_subscribedEvents.insert(EventType::PlayerHitAmmoPickup);
        m_subscribedEvents.insert(EventType::PlayerHitHealthPickup);
        m_subscribedEvents.insert(EventType::PlayerHitLightningPickup);
    }

    void HandleBulletHitEnemy(EntityManager& entityManager, EntityId bulletEntityId, EntityId enemyEntityId);
    void HandleEnemyHitPlayer(EntityManager& entityManager, EntityId enemyEntityId);
    void HandleBulletOutOfBounds(EntityManager& entityManager, EntityId bulletEntityId);
};
