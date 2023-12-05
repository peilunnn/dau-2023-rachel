#pragma once
#include "Components/include/Cooldown.h"
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"

class CooldownHandler : public System {
public:
    static CooldownHandler& GetInstance() {
        static CooldownHandler instance;
        return instance;
    }

    CooldownHandler(CooldownHandler const&) = delete;
    void operator=(CooldownHandler const&) = delete;

    void Update(float deltaTime) {
        auto& entityManager = EntityManager::GetInstance();
        for (auto entityId : entityManager.GetEntitiesWithComponents<Cooldown>()) {
            auto cooldown = entityManager.GetComponent<Cooldown>(entityId);
            if (cooldown) {
                cooldown->Update(deltaTime);
            }
        }
    }

private:
    CooldownHandler() {}
};
