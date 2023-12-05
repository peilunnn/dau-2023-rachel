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
    static SystemManager& GetInstance() {
        static SystemManager instance;
        return instance;
    }
    SystemManager(SystemManager const&) = delete;
    void operator=(SystemManager const&) = delete;

    void Init();
    void SendEvent(Event event);
    void ProcessEvents(float deltaTime);

private:
    SystemManager() = default;

    vector<System*> m_systems;
    queue<Event> m_eventQueue;
};