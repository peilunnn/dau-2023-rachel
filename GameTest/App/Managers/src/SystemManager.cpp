#include "stdafx.h"
#include "../include/SystemManager.h"
#include "Systems/include/SystemFactory.h"

void SystemManager::Init()
{
    vector<unique_ptr<System>> systems = SystemFactory::CreateAllSystems();
    for (unique_ptr<System>& system : systems) {
        AddSystem(std::move(system));
    }
}

void SystemManager::AddSystem(unique_ptr<System> system) {
    m_systems.push_back(move(system));
}

void SystemManager::SendEvent(Event event) {
    m_eventQueue.push(event);
}

void SystemManager::ProcessEvents(EntityManager& entityManager, ScoreHandler& scoreHandler, float deltaTime) {
    while (!m_eventQueue.empty()) {
        Event event = m_eventQueue.front();

        for (const auto& system : m_systems) {
            if (system->GetSubscribedEvents().count(event.eventType) > 0)
                system->HandleEvent(event, entityManager, deltaTime);
        }
        m_eventQueue.pop();
    }
}
