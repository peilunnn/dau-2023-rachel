#pragma once
#include "Components/include/Screen.h"
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"
using glm::vec2;

class MovementHandler : public System 
{
public:
    static MovementHandler& GetInstance();
    MovementHandler(MovementHandler const&) = delete;
    void operator=(MovementHandler const&) = delete;

    void Update(float deltaTime);
    void HandleEvent(const Event& event, float deltaTime) override;

private:
    MovementHandler() 
    {
        m_subscribedEvents.insert(EventType::BulletHitEnemy);
    }

    const float ENEMY_BOTTOM_OFFSET = -30.0f;
    const float ENEMY_TOP_OFFSET = 20.0f;
    const float HOMING_SPEED = 80.0f;
    const float PLAYER_BOTTOM_OFFSET = 30.0f;
    const float PLAYER_TOP_OFFSET = -30.0f;
    const float ZERO_POS = 0.0f;
    const vec2 ZERO_VELOCITY = vec2(0.0f, 0.0f);

    void UpdatePlayerMovement(EntityManager& entityManager, Screen& screen, EntityId entityId, float deltaTime);
    void UpdateEnemyMovement(EntityManager& entityManager, Screen& screen, EntityId entityId, float deltaTime);
    void UpdateStandardEnemyMovement(EntityManager& entityManager, Screen& screen, EntityId entityId, float deltaTime);
    void UpdateHomingEnemyMovement(EntityManager& entityManager, Screen& screen, EntityId entityId, float deltaTime);
    void UpdateBulletMovement(EntityManager& entityManager, Screen& screen, EntityId entityId, float deltaTime);
    void HandleBulletHitEnemy(EntityManager& entityManager, EntityId enemyEntityId, float deltaTime);
};
