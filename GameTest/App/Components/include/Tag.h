#ifndef TAG_H
#define TAG_H

#include <string>
#include "Component.h"

enum class EntityType {
    NONE,
    PLAYER,
    ENEMY,
    BULLET,
    RELOADING_CIRCLE,
    AMMO_EMPTY,
    AMMO_FILLED,
    HEALTH_BAR
};

enum class EntityState {
    ALIVE,
    HIT_BY_BULLET, // For enemies
    HIT_BY_ENEMY,  // For player
};


class Tag : public Component {
public:
    EntityType entityType;
    EntityState entityState;

    Tag(EntityType type = EntityType::NONE, EntityState initialState = EntityState::ALIVE)
        : entityType(type), entityState(initialState) {}
};

#endif // TAG_H
