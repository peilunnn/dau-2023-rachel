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
#include "Systems/include/TimerHandler.h"
#include "Systems/include/CooldownHandler.h"

void SystemManager::Init()
{
    m_systems.push_back(&HealthHandler::GetInstance());
    m_systems.push_back(&MovementHandler::GetInstance());
    m_systems.push_back(&CollisionHandler::GetInstance());
    m_systems.push_back(&InputHandler::GetInstance());
    m_systems.push_back(&ShootingHandler::GetInstance());
    m_systems.push_back(&RenderingHandler::GetInstance());
    m_systems.push_back(&ScreenHandler::GetInstance());
    m_systems.push_back(&ScoreHandler::GetInstance());
    m_systems.push_back(&EntityHandler::GetInstance());
    m_systems.push_back(&AnimationHandler::GetInstance());
    m_systems.push_back(&TimerHandler::GetInstance());
    m_systems.push_back(&CooldownHandler::GetInstance());
}

void SystemManager::SendEvent(Event event) {
    m_eventQueue.push(event);
}

void SystemManager::ProcessEvents(float deltaTime) {
    EntityManager& entityManager = EntityManager::GetInstance();
    ScoreHandler& scoreHandler = ScoreHandler::GetInstance();

    while (!m_eventQueue.empty()) {
        Event event = m_eventQueue.front();

        for (System* system : m_systems) {
            if (system->GetSubscribedEvents().count(event.GetEventType()) > 0)
                system->HandleEvent(event, deltaTime);
        }
        m_eventQueue.pop();
    }
}
