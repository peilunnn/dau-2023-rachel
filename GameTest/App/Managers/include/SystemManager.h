#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <queue>
#include "EntityManager.h"
#include "../../Systems/include/System.h"
#include "../../Systems/include/SystemEvent.h"
#include "../../Systems/include/AnimationHandler.h"

class SystemManager {
private:
    std::vector<std::unique_ptr<System>> systems;

public:
    std::queue<SystemEvent> eventQueue;

    void AddSystem(std::unique_ptr<System> system);
    void SendEvent(SystemEvent event);
    void ProcessEvents(EntityManager& entityManager, float deltaTime, const glm::vec3& playerPos, float screenWidth, float screenHeight);
};

#endif SYSTEM_MANAGER_H
