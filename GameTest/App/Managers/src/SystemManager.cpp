#include "stdafx.h"
#include "../include/SystemManager.h"

void SystemManager::AddSystem(std::unique_ptr<System> system) {
    systems.push_back(std::move(system));
}

void SystemManager::SendEvent(Event event) {
    eventQueue.push(event);
}

void SystemManager::ProcessEvents(EntityManager& entityManager, float deltaTime, const glm::vec3& playerPos, float screenWidth, float screenHeight) {
    while (!eventQueue.empty()) {
        Event event = eventQueue.front();

        switch (event.eventType) {
        case EventType::BulletHitEnemy:
            HandleBulletHitEnemyEvent(entityManager, event, deltaTime, playerPos, screenWidth, screenHeight);
            break;
        }
        // Handle PlayerHitEnemy event
        // Handle PlayerHitReloadingCircle event
        // Handle PlayerHealthReachZero event
        // Handle CountdownReachZero event

        eventQueue.pop();
    }
}

void SystemManager::HandleBulletHitEnemyEvent(EntityManager& entityManager, const Event& event, float deltaTime, const glm::vec3& playerPos, float screenWidth, float screenHeight)
{
    for (const auto& system : systems) {
        if (system->GetSystemType() == System::Type::AnimationHandler) {
            auto animationHandler = dynamic_cast<AnimationHandler*>(system.get());
            if (animationHandler && event.entities.size() == 2)
                animationHandler->ProcessBulletHitEnemy(entityManager, event.entities[0], event.entities[1], deltaTime);
        }
    }

    entityManager.ProcessBulletHitEnemy(entityManager, deltaTime, event, playerPos, screenWidth, screenHeight);
}
