#pragma once
#include "System.h"

class TimerHandler : public System {
public:
    static TimerHandler& GetInstance();
    TimerHandler(TimerHandler const&) = delete;
    void operator=(TimerHandler const&) = delete;

    void Update(float deltaTime);

private:
    TimerHandler() = default;
};