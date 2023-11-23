#ifndef TAG_H
#define TAG_H

#include <string>
#include "Component.h"

enum class EntityType {
    PLAYER,
    ENEMY,
    BULLET,
};

enum class EntityState {
    ALIVE,
    HIT_BY_BULLET, // For enemies
    HIT_BY_ENEMY,  // For player
};


class Tag : public Component {
public:
    EntityType entityType;
    EntityState state;

    Tag(EntityType type = EntityType::ENEMY, EntityState initialState = EntityState::ALIVE)
        : entityType(type), state(initialState) {}
};

#endif // TAG_H
