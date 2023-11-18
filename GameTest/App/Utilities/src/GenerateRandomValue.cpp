#include "stdafx.h"
#include "../include/GenerateRandomValue.h"

float RandomUtility::GenerateFloat(float min, float max) {
    static std::random_device rd;
    static std::mt19937 eng(rd());
    std::uniform_real_distribution<> distr(min, max);
    return distr(eng);
}

glm::vec2 RandomUtility::GenerateVec2(float minX, float maxX, float minY, float maxY) {
    return glm::vec2(GenerateFloat(minX, maxX), GenerateFloat(minY, maxY));
}

glm::vec3 RandomUtility::GenerateVec3(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {
    return glm::vec3(GenerateFloat(minX, maxX), GenerateFloat(minY, maxY), GenerateFloat(minZ, maxZ));
}