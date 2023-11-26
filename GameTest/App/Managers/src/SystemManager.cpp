#include "stdafx.h"
#include "../include/SystemManager.h"
#include "../../Systems/include/AnimationHandler.h"
#include "../../Systems/include/InputHandler.h"
#include "../../Systems/include/MovementHandler.h"
#include "../../Systems/include/RenderingHandler.h"
#include "../../Systems/include/CollisionHandler.h"
#include "../../Systems/include/HealthHandler.h"
#include "../../Systems/include/ScreenHandler.h"
#include "../../Systems/include/ScoreHandler.h"

void SystemManager::Init()
{
    AddSystem(std::make_unique<AnimationHandler>());
    AddSystem(std::make_unique<CollisionHandler>());
    AddSystem(std::make_unique<InputHandler>());
    AddSystem(std::make_unique<MovementHandler>());
    AddSystem(std::make_unique<ShootingHandler>());
    AddSystem(std::make_unique<RenderingHandler>());
    AddSystem(std::make_unique<HealthHandler>());
    AddSystem(std::make_unique<ScreenHandler>());
    AddSystem(std::make_unique<ScoreHandler>());
}

void SystemManager::AddSystem(std::unique_ptr<System> system) {
    systems.push_back(std::move(system));
}

void SystemManager::SendEvent(Event event) {
    eventQueue.push(event);
}

void SystemManager::ProcessEvents(EntityManager& entityManager, ScoreHandler& scoreHandler, float deltaTime, const glm::vec3& playerPos) {
    float screenWidth = ScreenHandler::SCREEN_WIDTH;
    float screenHeight = ScreenHandler::SCREEN_HEIGHT;
    
    while (!eventQueue.empty()) {
        Event event = eventQueue.front();

        switch (event.eventType)
        {
        case EventType::BulletHitEnemy:
            HandleBulletHitEnemyEvent(entityManager, scoreHandler, event, deltaTime, playerPos);
            break;
        case EventType::EnemyHitPlayer:
            HandleEnemyHitPlayerEvent(entityManager, event, deltaTime);
            break;
        case EventType::PlayerHitReloadingCircle:
            HandlePlayerHitPlayerReloadingCircle(entityManager, event, deltaTime);
            break;
            // Handle PlayerHealthReachZero event
            // Handle CountdownReachZero event
        }
        eventQueue.pop();
    }
}

void SystemManager::HandleBulletHitEnemyEvent(EntityManager& entityManager, ScoreHandler& scoreHandler, const Event& event, float deltaTime, const glm::vec3& playerPos)
{
    float screenWidth = ScreenHandler::SCREEN_WIDTH;
    float screenHeight = ScreenHandler::SCREEN_HEIGHT;

    for (const auto& system : systems) {
        if (system->GetSystemType() == SystemType::AnimationHandler) {
            auto animationHandler = dynamic_cast<AnimationHandler*>(system.get());
            
            if (!(animationHandler && event.entities.size() == 2))
                return;
            
            animationHandler->ProcessBulletHitEnemy(entityManager, event.entities[0], event.entities[1], deltaTime);
        }
    }

    entityManager.ProcessBulletHitEnemy(entityManager, event, deltaTime, playerPos, screenWidth, screenHeight);
    scoreHandler.ProcessBulletHitEnemy(entityManager, deltaTime);
}

void SystemManager::HandleEnemyHitPlayerEvent(EntityManager& entityManager, const Event& event, float deltaTime)
{
    // We do two separate loops because order is important - we want to update health before updating animation
    for (const auto& system : systems) {
        if (system->GetSystemType() == SystemType::HealthHandler) {
            auto healthHandler = dynamic_cast<HealthHandler*>(system.get());

            if (!(healthHandler && event.entities.size() == 2))
                return;

            healthHandler->ProcessEnemyHitPlayer(entityManager);
            break;
        }
    }

    for (const auto& system : systems) {
        if (system->GetSystemType() == SystemType::AnimationHandler)
        {
            auto animationHandler = dynamic_cast<AnimationHandler*>(system.get());

            if (!(animationHandler && event.entities.size() == 2))
                return;

            animationHandler->ProcessEnemyHitPlayer(entityManager, deltaTime);
            break;
        }
    }

    entityManager.ProcessEnemyHitPlayer(entityManager, event, deltaTime);
}

void SystemManager::HandlePlayerHitPlayerReloadingCircle(EntityManager& entityManager, const Event& event, float deltaTime)
{
    for (const auto& system : systems) {
        if (system->GetSystemType() == SystemType::ShootingHandler)
        {
            auto shootingHandler = dynamic_cast<ShootingHandler*>(system.get());

            if (!(shootingHandler && event.entities.size() == 2))
                return;

            shootingHandler->ProcessPlayerHitReloadingCircle(entityManager, deltaTime);
            break;
        }
    }
}
