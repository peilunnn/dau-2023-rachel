#pragma once
#include <Windows.h>
#include <string>
#include <iostream>
#include <sstream>
#include <random>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "SimpleSprite.h"
#include "../App/Components/include/Tag.h"
#include "../App/Utilities/include/Enums.h"

struct SpriteDimensions {
    float width;
    float height;
    float adjustedWidth; // width after applying the multiplier
    float adjustedHeight; // height after applying the multiplier
};

class Helper {
public:
    // For getting sprites and sprite sheets
    static inline const char* PATH_TO_PLAYER_SPRITE_SHEET = ".\\Data\\SpriteSheets\\Player.bmp";
    static inline const char* PATH_TO_ENEMY_SPRITE_SHEET = ".\\Data\\SpriteSheets\\Enemy.png";
    static inline const char* PATH_TO_BULLET_SPRITE = ".\\Data\\Sprites\\Bullet.bmp";
    static inline const char* PATH_TO_RELOADING_CIRCLE_SPRITE_SHEET = ".\\Data\\SpriteSheets\\ReloadingCircle.png";
    static inline const char* PATH_TO_AMMO_EMPTY_SPRITE = ".\\Data\\Sprites\\AmmoEmpty.png";
    static inline const char* PATH_TO_AMMO_FILLED_SPRITE = ".\\Data\\Sprites\\AmmoFilled.png";
    static inline const char* PATH_TO_HEALTH_BAR_SPRITE_SHEET = ".\\Data\\SpriteSheets\\HealthBar.png";

    // Getting string representation of an EntityType (for reading Tag)
    static std::string GetEntityTypeString(EntityType entityType) {
        switch (entityType) {
        case EntityType::PLAYER:
            return "Player";
        case EntityType::ENEMY:
            return "Enemy";
        case EntityType::BULLET:
            return "Bullet";
        default:
            return "Unknown";
        }
    }

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
        return static_cast<float>(distr(eng));
    }

    static glm::vec2 GenerateVec2(float minX, float maxX, float minY, float maxY) {
        return glm::vec2(GenerateFloat(minX, maxX), GenerateFloat(minY, maxY));
    }

    static glm::vec3 GenerateVec3(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {
        return glm::vec3(GenerateFloat(minX, maxX), GenerateFloat(minY, maxY), GenerateFloat(minZ, maxZ));
    }
    
    // For spawning enemy in the opposite quadrant of player
    static glm::vec3 GetOppositeQuadrantPosition(const glm::vec3& playerPos, float screenWidth, float screenHeight);

    // For dealing with entities going out of bounds
    static SpriteDimensions GetSpriteDimensions(std::shared_ptr<CSimpleSprite> sprite, float multiplier);
};