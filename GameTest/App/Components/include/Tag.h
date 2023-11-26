#pragma once
#include "../App/Utilities/include/Enums.h"
#include "Component.h"

class Tag : public Component {
public:
	EntityType entityType = EntityType::NONE;
	EntityState entityState = EntityState::ALIVE;

	Tag(EntityType type) : entityType(type) {}
};
