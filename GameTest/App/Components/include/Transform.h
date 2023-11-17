#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/vec3.hpp>

class TransformComponent {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    TransformComponent() : position(0.0f), rotation(0.0f), scale(1.0f) {}
};

#endif // TRANSFORM_H
