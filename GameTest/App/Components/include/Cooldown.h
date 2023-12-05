#pragma once
#include "Component.h"

class Cooldown : public Component {
public:
    Cooldown(float initialCooldown = 0.0f)
        : m_totalCooldownTime(initialCooldown), m_TimeLeftInCooldown(0.0f), m_IsActive(false) {}

    bool IsCooldownComplete() const {
        return m_TimeLeftInCooldown <= 0.0f;
    }

    void StartCooldown() {
        m_TimeLeftInCooldown = m_totalCooldownTime;
        m_IsActive = true;
    }

    void Update(float deltaTime) {
        if (m_IsActive && m_TimeLeftInCooldown > 0.0f) {
            m_TimeLeftInCooldown -= deltaTime;
            if (m_TimeLeftInCooldown <= 0.0f) {
                m_IsActive = false;
            }
        }
    }

    bool IsActive() const { return m_IsActive; }

private:
    const float m_totalCooldownTime;
    float m_TimeLeftInCooldown;
    bool m_IsActive;
};
