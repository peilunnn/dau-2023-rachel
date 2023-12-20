#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"

class EntityHandler : public System
{
public:
    static EntityHandler &GetInstance();
    EntityHandler(EntityHandler const &) = delete;
    void operator=(EntityHandler const &) = delete;

    void HandleEvent(const Event &event, float deltaTime) override;
    void InitEnemy();
    void MoveEntityToRandomPos(EntityId entityId);

    int GetEnemiesToStrike() const { return m_enemiesToStrike; }
    void SetEnemiesToStrike(int newEnemiesToStrike) { m_enemiesToStrike = newEnemiesToStrike; }

private:
    EntityHandler()
    {
        m_subscribedEvents.insert(EventType::BulletHitEnemy);
        m_subscribedEvents.insert(EventType::EnemyHitPlayer);
        m_subscribedEvents.insert(EventType::BulletOutOfBounds);
        m_subscribedEvents.insert(EventType::PlayerHitAmmoPickup);
        m_subscribedEvents.insert(EventType::PlayerHitHealthPickup);
        m_subscribedEvents.insert(EventType::PlayerHitLightningPickup);
    }

    vector<EntityId> m_activeEnemies = vector<EntityId>();
    int m_enemiesToStrike = 0;
    const float MAX_VX = 300.0f;
    const float MAX_VY = 300.0f;
    const float MIN_VX = -100.0f;
    const float MIN_VY = -100.0f;
    const float OFFSET = 20.0f;
    const float ZERO_POS_FLOAT = 0.0f;
    const int MAX_ENEMIES_TO_STRIKE = 5;
    const vec2 ZERO_VELOCITY = vec2(0.0f);

    void HandleBulletHitEnemy(EntityManager &entityManager, EntityId bulletEntityId, EntityId enemyEntityId);
    void HandleEnemyHitPlayer(EntityManager &entityManager, EntityId enemyEntityId);
    void HandleBulletOutOfBounds(EntityManager &entityManager, EntityId bulletEntityId);
    void SetEnemiesToStrike(EntityManager& entityManager);
    void InitLightningStrikes(EntityManager& entityManager);
    void EmitSteamParticles();
};
