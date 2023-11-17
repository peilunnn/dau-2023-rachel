#ifndef BUTTON_H
#define BUTTON_H

#include <functional>
#include "Component.h"

struct Button : public Component {
    bool isHovered;
    bool isPressed;
    std::function<void()> onClick;

    Button(std::function<void()> click) : isHovered(false), isPressed(false), onClick(click) {}
};

#endif // BUTTON_H
