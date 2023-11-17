#ifndef BUTTON_H
#define BUTTON_H

#include <functional>

struct Button {
    bool isHovered;
    bool isPressed;
    std::function<void()> onClick;

    Button(std::function<void()> click) : isHovered(false), isPressed(false), onClick(click) {}
};

#endif // BUTTON_H
