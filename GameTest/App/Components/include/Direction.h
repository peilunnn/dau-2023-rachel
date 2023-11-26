#pragma once
#include "Component.h"

class Direction : public Component {
public:
    bool bounced = false;

    Direction() = default;
};