#pragma once
#include "Component.h"

class Health : public Component {
public:
	int currentHealth = 100;
	int maxHealth = 100;

	Health() = default;
};