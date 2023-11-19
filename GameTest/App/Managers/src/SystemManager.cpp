#include "stdafx.h"
#include "../include/SystemManager.h"

void SystemManager::AddSystem(std::unique_ptr<System> system) {
    systems.push_back(std::move(system));
}

void SystemManager::SendEvent(SystemEvent event) {
    eventQueue.push(event);
}

void SystemManager::ProcessEvents(EntityManager& entityManager, float deltaTime) {
    while (!eventQueue.empty()) {
        SystemEvent event = eventQueue.front();
        eventQueue.pop();

        for (const auto& system : systems) {
            if (system->GetSystemType() == System::Type::HandleAnimation && event.type == SystemEvent::BulletHitEnemy) {
                auto handleAnimationSystem = dynamic_cast<HandleAnimation*>(system.get());
                if (handleAnimationSystem) {
                    handleAnimationSystem->ProcessBulletHitEnemy(entityManager, event.entity, deltaTime);
                }
                break;
            }
            // Handle PlayerHitEnemy event
            // Handle PlayerHitReloadingCircle event
            // Handle PlayerHealthReachZero event
            // Handle CountdownReachZero event
        }
    }
}