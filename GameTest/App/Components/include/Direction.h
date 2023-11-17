#ifndef DIRECTION_H
#define DIRECTION_H

#include <glm/vec2.hpp>
#include "Component.h"

class Direction : public Component {
public:
    glm::vec2 direction;
    bool isBouncing;

    Direction() : direction(0.0f), isBouncing(false) {}
};

#endif // DIRECTION_H
