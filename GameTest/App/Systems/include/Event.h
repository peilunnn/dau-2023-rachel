#pragma once
#include <vector>
#include "../App/Utilities/include/Enums.h"
#include "../App/Components/include/EntityID.h"

class Event {
public:
    EventType eventType;
    std::vector<EntityId> entities;

    Event(EventType type, std::initializer_list<EntityId> ents, bool newEnemies = false)
        : eventType(type), entities(ents) {}
};