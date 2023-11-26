#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <queue>
#include "EntityManager.h"
#include "../../Systems/include/System.h"
#include "../../Systems/include/Event.h"
#include "../../Systems/include/ScreenHandler.h"
#include "../../Systems/include/ScoreHandler.h"

class SystemManager {
private:
    std::vector<std::unique_ptr<System>> systems;

public:
    std::queue<Event> eventQueue;
    
    void Init();
    void AddSystem(std::unique_ptr<System> system);
    void SendEvent(Event event);
    void ProcessEvents(EntityManager& entityManager, ScoreHandler& scoreHandler, float deltaTime, const glm::vec3& playerPos);
    void HandleBulletHitEnemyEvent(EntityManager& entityManager, ScoreHandler& scoreHandler, const Event& event, float deltaTime, const glm::vec3& playerPos);
    void HandleEnemyHitPlayerEvent(EntityManager& entityManager, const Event& event, float deltaTime);
    void HandlePlayerHitPlayerReloadingCircle(EntityManager& entityManager, const Event& event, float deltaTime);
};

#endif SYSTEM_MANAGER_H
