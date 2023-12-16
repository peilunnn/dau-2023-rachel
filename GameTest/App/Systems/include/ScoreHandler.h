#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"
#include <set>

class ScoreHandler : public System {
public:
    static ScoreHandler& GetInstance();
    ScoreHandler(ScoreHandler const&) = delete;
    void operator=(ScoreHandler const&) = delete;

    void HandleEvent(const Event& event, float deltaTime) override;
    void ResetScore();

private:
    ScoreHandler() 
    {
        m_subscribedEvents.insert(EventType::BulletHitEnemy);
        m_subscribedEvents.insert(EventType::PlayerHitLightningPickup);
    }

    void HandleBulletHitEnemy(EntityManager& entityManager, float deltaTime);
    void HandlePlayerHitLightningPickup(EntityManager& entityManager, float deltaTime);
};
