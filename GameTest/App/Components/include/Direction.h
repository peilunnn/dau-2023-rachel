#ifndef DIRECTION_H
#define DIRECTION_H

#include <glm/vec2.hpp>
#include "Component.h"

class Direction : public Component {
public:
    bool bounced;

    Direction() : bounced(false) {}
};

#endif // DIRECTION_H
