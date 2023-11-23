#include "stdafx.h"
#include "../include/SystemManager.h"

void SystemManager::AddSystem(std::unique_ptr<System> system) {
    systems.push_back(std::move(system));
}

void SystemManager::SendEvent(SystemEvent event) {
    eventQueue.push(event);
}

void SystemManager::ProcessEvents(EntityManager& entityManager, float deltaTime, const glm::vec3& playerPos, float screenWidth, float screenHeight) {
    while (!eventQueue.empty()) {
        SystemEvent event = eventQueue.front();

        for (const auto& system : systems) {
            if (system->GetSystemType() == System::Type::AnimationHandler && event.type == SystemEvent::BulletHitEnemy) {
                auto animationHandler = dynamic_cast<AnimationHandler*>(system.get());
                if (animationHandler) {
                    animationHandler->ProcessBulletHitEnemy(entityManager, event.entity1, event.entity2, deltaTime);
                }

                if (!event.newEnemiesCreated) {
                    entityManager.ProcessBulletHitEnemy(entityManager, deltaTime, event, playerPos, screenWidth, screenHeight);
                    event.newEnemiesCreated = true;
                }

                break;
            }
            // Handle PlayerHitEnemy event
            // Handle PlayerHitReloadingCircle event
            // Handle PlayerHealthReachZero event
            // Handle CountdownReachZero event
        }

        eventQueue.pop();
    }
}