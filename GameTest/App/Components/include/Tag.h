#pragma once
#include <string>
#include "Component.h"
#include "../App/Utilities/include/Enums.h"

class Tag : public Component {
public:
    EntityType entityType = EntityType::NONE;
    EntityState entityState = EntityState::ALIVE;

    Tag(EntityType type) : entityType(type) {}
};
