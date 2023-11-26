#pragma once
#include <glm/vec2.hpp>
#include "Component.h"

class Direction : public Component {
public:
    bool bounced;

    Direction() : bounced(false) {}
};