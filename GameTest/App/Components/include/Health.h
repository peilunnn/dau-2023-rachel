#pragma once
#include "Components/include/Component.h"

class Health : public Component 
{
public:
    Health() = default;

    int GetCurrentHealth() const { return m_currentHealth; }
    int GetMaxHealth() const { return m_maxHealth; }

    void SetCurrentHealth(int newHealth) { m_currentHealth = newHealth; }

private:
    int m_currentHealth = 100;
    const int m_maxHealth = 100;
};
