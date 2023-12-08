#pragma once
#include "Systems/include/Event.h"
#include "Systems/include/System.h"
#include <queue>
#include <vector>
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

    vector<System*> m_systems = vector<System*>();
    queue<Event> m_eventQueue = queue<Event>();
};