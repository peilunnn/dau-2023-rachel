#ifndef BAR_H
#define BAR_H

#include <glm/vec3.hpp>
#include "Component.h"

struct Bar : public Component {
    int currentValue;
    int maxValue;
    glm::vec3 foregroundColor;
    glm::vec3 backgroundColor;

    Bar(int current, int max, const glm::vec3& fgColor, const glm::vec3& bgColor)
        : currentValue(current), maxValue(max), foregroundColor(fgColor), backgroundColor(bgColor) {}
};

#endif // BAR_H
