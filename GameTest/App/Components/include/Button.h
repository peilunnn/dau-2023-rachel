#pragma once
#include <functional>
#include "Component.h"

struct Button : public Component {
    bool isHovered;
    bool isPressed;
    std::function<void()> onClick;

    Button(std::function<void()> click) : isHovered(false), isPressed(false), onClick(click) {}
};