#pragma once
#include <vector>
#include "../App/Utilities/include/Enums.h"

using Entity = unsigned int;

class Event {
public:
    EventType eventType;
    std::vector<Entity> entities;

    Event(EventType type, std::initializer_list<Entity> ents, bool newEnemies = false)
        : eventType(type), entities(ents) {}
};