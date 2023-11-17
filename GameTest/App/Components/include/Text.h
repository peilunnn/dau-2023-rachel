#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <glm/vec3.hpp>
#include "Component.h"

struct Text : public Component {
    std::string text;
    glm::vec3 color;
    int fontSize;

    Text(const std::string& txt, const glm::vec3& col, int size)
        : text(txt), color(col), fontSize(size) {}
};

#endif // TEXT_H
