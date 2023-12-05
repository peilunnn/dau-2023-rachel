#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"
#include <string>
#include <set>

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
