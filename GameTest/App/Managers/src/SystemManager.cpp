#include "stdafx.h"
#include "../include/SystemManager.h"
#include "Systems/include/AnimationHandler.h"
#include "Systems/include/InputHandler.h"
#include "Systems/include/MovementHandler.h"
#include "Systems/include/RenderingHandler.h"
#include "Systems/include/CollisionHandler.h"
#include "Systems/include/HealthHandler.h"
#include "Systems/include/ScreenHandler.h"
#include "Systems/include/ScoreHandler.h"
#include "Systems/include/ShootingHandler.h"

void SystemManager::Init()
{
    AddSystem(make_unique<AnimationHandler>());
    AddSystem(make_unique<CollisionHandler>());
    AddSystem(make_unique<InputHandler>());
    AddSystem(make_unique<MovementHandler>());
    AddSystem(make_unique<ShootingHandler>());
    AddSystem(make_unique<RenderingHandler>());
    AddSystem(make_unique<HealthHandler>());
    AddSystem(make_unique<ScreenHandler>());
    AddSystem(make_unique<ScoreHandler>());
}

void SystemManager::AddSystem(unique_ptr<System> system) {
    m_systems.push_back(move(system));
}

void SystemManager::SendEvent(Event event) {
    m_eventQueue.push(event);
}

void SystemManager::ProcessEvents(EntityManager& entityManager, ScoreHandler& scoreHandler, float deltaTime, const glm::vec3& playerPos) {
    constexpr float screenWidth = ScreenHandler::SCREEN_WIDTH;
    constexpr float screenHeight = ScreenHandler::SCREEN_HEIGHT;
    
    while (!m_eventQueue.empty()) {
        Event event = m_eventQueue.front();

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
        m_eventQueue.pop();
    }
}

void SystemManager::HandleBulletHitEnemyEvent(EntityManager& entityManager, ScoreHandler& scoreHandler, const Event& event, float deltaTime, const glm::vec3& playerPos)
{
    constexpr float screenWidth = ScreenHandler::SCREEN_WIDTH;
    constexpr float screenHeight = ScreenHandler::SCREEN_HEIGHT;

    for (const auto& system : m_systems) {
        if (system->GetSystemType() == SystemType::AnimationHandler) {
            AnimationHandler* animationHandler = dynamic_cast<AnimationHandler *>(system.get());

            if (!(animationHandler && event.entities.size() == 2))
                return;
            
            animationHandler->HandleBulletHitEnemy(entityManager, event.entities[0], event.entities[1], deltaTime);
        }
    }

    entityManager.HandleBulletHitEnemy(event, deltaTime, playerPos, screenWidth, screenHeight);
    scoreHandler.HandleBulletHitEnemy(entityManager, deltaTime);
}

void SystemManager::HandleEnemyHitPlayerEvent(EntityManager& entityManager, const Event& event, float deltaTime)
{
    // We do two separate loops because order is important - we want to update health before updating animation
    for (const auto& system : m_systems) {
        if (system->GetSystemType() == SystemType::HealthHandler) {
            auto healthHandler = dynamic_cast<HealthHandler*>(system.get());

            if (!(healthHandler && event.entities.size() == 2))
                return;

            healthHandler->HandleEnemyHitPlayer(entityManager);
            break;
        }
    }

    for (const auto& system : m_systems) {
        if (system->GetSystemType() == SystemType::AnimationHandler)
        {
            auto animationHandler = dynamic_cast<AnimationHandler*>(system.get());

            if (!(animationHandler && event.entities.size() == 2))
                return;

            animationHandler->HandleEnemyHitPlayer(entityManager, deltaTime);
            break;
        }
    }

    entityManager.HandleEnemyHitPlayer(entityManager, event, deltaTime);
}

void SystemManager::HandlePlayerHitPlayerReloadingCircle(EntityManager& entityManager, const Event& event, float deltaTime)
{
    for (const auto& system : m_systems) {
        if (system->GetSystemType() == SystemType::ShootingHandler)
        {
            ShootingHandler* shootingHandler = dynamic_cast<ShootingHandler*>(system.get());

            if (!(shootingHandler && event.entities.size() == 2))
                return;

            shootingHandler->ProcessPlayerHitReloadingCircle(entityManager, deltaTime);
            break;
        }
    }
}
