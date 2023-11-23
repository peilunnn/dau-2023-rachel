#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <queue>
#include "EntityManager.h"
#include "../../Systems/include/System.h"
#include "../../Systems/include/AnimationHandler.h"

class SystemManager {
private:
    std::vector<std::unique_ptr<System>> systems;

public:
    struct SystemEvent {
        enum EventType { BulletHitEnemy, PlayerHitEnemy, PlayerHitReloadingCircle, PlayerHealthReachZero, CountdownReachZero } type;
        std::vector<Entity> entities;
        bool newEnemiesCreated = false;
    };
    std::queue<SystemEvent> eventQueue;

    void AddSystem(std::unique_ptr<System> system);
    void SendEvent(SystemEvent event);
    void ProcessEvents(EntityManager& entityManager, float deltaTime, std::shared_ptr<CSimpleSprite> enemySprite, const glm::vec3& playerPos, float screenWidth, float screenHeight);
};

#endif SYSTEM_MANAGER_H
