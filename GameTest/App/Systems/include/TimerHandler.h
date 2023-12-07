#pragma once
#include "System.h"

class TimerHandler : public System {
public:
    static TimerHandler& GetInstance() {
        static TimerHandler instance;
        return instance;
    }
    TimerHandler(TimerHandler const&) = delete;
    void operator=(TimerHandler const&) = delete;

    void Update(float deltaTime);

private:
    TimerHandler() = default;
};