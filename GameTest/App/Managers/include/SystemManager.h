#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <queue>
#include "EntityManager.h"
#include "../../Systems/include/System.h"
#include "../../Systems/include/Event.h"

class SystemManager {
private:
    std::vector<std::unique_ptr<System>> systems;

public:
    std::queue<Event> eventQueue;
    
    void Init();
    void AddSystem(std::unique_ptr<System> system);
    void SendEvent(Event event);
    void ProcessEvents(EntityManager& entityManager, float deltaTime, const glm::vec3& playerPos, float screenWidth, float screenHeight);
    void HandleBulletHitEnemyEvent(EntityManager& entityManager, const Event& event, float deltaTime, const glm::vec3& playerPos, float screenWidth, float screenHeight);
};

#endif SYSTEM_MANAGER_H
