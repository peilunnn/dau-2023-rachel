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
            if (system->GetSystemType() == System::Type::AnimationHandler && event.type == SystemEvent::BulletHitEnemy) {
                auto animationHandler = dynamic_cast<AnimationHandler*>(system.get());
                if (animationHandler) {
                    animationHandler->ProcessBulletHitEnemy(entityManager, event.entity, deltaTime);
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