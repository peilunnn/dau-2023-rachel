#ifndef TAG_H
#define TAG_H

#include <string>
#include "Component.h"

enum class EntityType {
    PLAYER,
    ENEMY,
    BULLET,
};

class Tag : public Component {
public:
    EntityType entityType;

    Tag(EntityType type = EntityType::ENEMY) : entityType(type) {}
};

#endif // TAG_H
