#ifndef HELPER_H
#define HELPER_H

#include <Windows.h>
#include <string>
#include <iostream>
#include <sstream>
#include <random>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "SimpleSprite.h"

struct SpriteDimensions {
    float width;
    float height;
    float adjustedWidth; // width after applying the multiplier
    float adjustedHeight; // height after applying the multiplier
};

class Helper {
public:
    // Logging functions
    template<typename T>
    static void Log(const std::string& message, T value) {
        std::stringstream ss;
        ss << message << value << "\n";
        OutputDebugStringA(ss.str().c_str());
    }

    static void Log(const std::string& message) {
        OutputDebugStringA((message + "\n").c_str());
    }

    // Random value generation functions
    static float GenerateFloat(float min, float max) {
        static std::random_device rd;
        static std::mt19937 eng(rd());
        std::uniform_real_distribution<> distr(min, max);
        return distr(eng);
    }

    static glm::vec2 GenerateVec2(float minX, float maxX, float minY, float maxY) {
        return glm::vec2(GenerateFloat(minX, maxX), GenerateFloat(minY, maxY));
    }

    static glm::vec3 GenerateVec3(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {
        return glm::vec3(GenerateFloat(minX, maxX), GenerateFloat(minY, maxY), GenerateFloat(minZ, maxZ));
    }

    // Out-of-bounds functions
    static bool IsOutOfBounds(float x, float y, float screenWidth, float screenHeight) {
        return (x < 0 || x > screenWidth || y < 0 || y > screenHeight);
    }
    static SpriteDimensions GetSpriteDimensions(CSimpleSprite* sprite, float multiplier);
};

#endif // HELPER_H
