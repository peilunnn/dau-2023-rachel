#pragma once
#include "Component.h"

class Health : public Component {
public:
    int currentHealth;
    int maxHealth;

    Health() : currentHealth(100), maxHealth(100) {}
};