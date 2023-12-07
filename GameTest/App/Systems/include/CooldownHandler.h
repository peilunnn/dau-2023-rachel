#pragma once
#include "Systems/include/System.h"

class CooldownHandler : public System {
public:
    static CooldownHandler& GetInstance() {
        static CooldownHandler instance;
        return instance;
    }
    CooldownHandler(CooldownHandler const&) = delete;
    void operator=(CooldownHandler const&) = delete;

    void Update(float deltaTime);

private:
    CooldownHandler() = default;
};
