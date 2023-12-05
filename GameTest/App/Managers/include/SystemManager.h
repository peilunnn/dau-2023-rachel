#pragma once
#include "Systems/include/Event.h"
#include "Systems/include/ScoreHandler.h"
#include "Systems/include/ScreenHandler.h"
#include "Systems/include/System.h"
#include "EntityManager.h"
#include <queue>
#include <vector>
#include <memory>
#include <optional>
using namespace std;

class SystemManager {
public:
    void Init();
    void AddSystem(unique_ptr<System> system);
    void SendEvent(Event event);
    void ProcessEvents(EntityManager& entityManager, ScoreHandler& scoreHandler, float deltaTime);

private:
    vector<unique_ptr<System>> m_systems;
    queue<Event> m_eventQueue;
};