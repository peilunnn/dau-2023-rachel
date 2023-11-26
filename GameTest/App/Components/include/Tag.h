#pragma once
#include <string>
#include "Component.h"
#include "../App/Utilities/include/Enums.h"

class Tag : public Component {
public:
    EntityType entityType;
    EntityState entityState;

    Tag(EntityType type = EntityType::NONE, EntityState initialState = EntityState::ALIVE)
        : entityType(type), entityState(initialState) {}
};