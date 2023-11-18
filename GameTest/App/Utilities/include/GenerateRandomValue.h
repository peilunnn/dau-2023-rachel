#ifndef GENERATE_RANDOM_VALUE_H
#define GENERATE_RANDOM_VALUE_H

#include <random>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class RandomUtility {
public:
    static float GenerateFloat(float min, float max);
    static glm::vec2 GenerateVec2(float minX, float maxX, float minY, float maxY);
    static glm::vec3 GenerateVec3(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
};

#endif // GENERATE_RANDOM_VALUE_H
