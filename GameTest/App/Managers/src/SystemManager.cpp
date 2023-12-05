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
#include "Systems/include/EntityHandler.h"

void SystemManager::Init()
{
    m_systems.push_back(make_unique<HealthHandler>());
    m_systems.push_back(make_unique<MovementHandler>());
    m_systems.push_back(make_unique<CollisionHandler>());
    m_systems.push_back(make_unique<InputHandler>());
    m_systems.push_back(make_unique<ShootingHandler>());
    m_systems.push_back(make_unique<RenderingHandler>());
    m_systems.push_back(make_unique<ScreenHandler>());
    m_systems.push_back(make_unique<ScoreHandler>());
    m_systems.push_back(make_unique<EntityHandler>());
    m_systems.push_back(make_unique<AnimationHandler>());
}

void SystemManager::SendEvent(Event event) {
    m_eventQueue.push(event);
}

void SystemManager::ProcessEvents(EntityManager& entityManager, ScoreHandler& scoreHandler, float deltaTime) {
    while (!m_eventQueue.empty()) {
        Event event = m_eventQueue.front();

        for (const auto& system : m_systems) {
            if (system->GetSubscribedEvents().count(event.GetEventType()) > 0)
                system->HandleEvent(event, entityManager, deltaTime);
        }
        m_eventQueue.pop();
    }
}
