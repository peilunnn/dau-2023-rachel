#pragma once
#include <functional>
#include "Component.h"

struct Button : public Component {
    bool isHovered = false;
    bool isPressed = false;
    std::function<void()> onClick;

    Button(std::function<void()> click) : onClick(click) {}
};