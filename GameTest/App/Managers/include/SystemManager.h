#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <queue>
#include "EntityManager.h"
#include "../../Systems/include/System.h"

class SystemManager {
private:
    std::vector<std::unique_ptr<System>> systems;

public:
    struct SystemEvent {
        enum EventType { BulletHitEnemy, PlayerHitEnemy, PlayerHitReloadingCircle, PlayerHealthReachZero, CountdownReachZero } type;
        Entity entity;
    };
    std::queue<SystemEvent> eventQueue;

    void addSystem(std::unique_ptr<System> system);
    
    void sendEvent(SystemEvent event);
    void processEvents();
};

#endif SYSTEM_MANAGER_H
