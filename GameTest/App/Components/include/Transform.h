#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/vec3.hpp>

class Transform {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scl)
        : position(pos), rotation(rot), scale(scl) {}
};

#endif // TRANSFORM_H
