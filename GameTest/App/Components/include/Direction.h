#ifndef DIRECTION_H
#define DIRECTION_H

#include <glm/vec2.hpp>

class Direction {
public:
    glm::vec2 direction;
    bool isBouncing;

    Direction() : direction(0.0f), isBouncing(false) {}
};

#endif // DIRECTION_H
