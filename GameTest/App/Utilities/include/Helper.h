#pragma once
#include "../App/Components/include/Tag.h"
#include "../App/Utilities/include/Enums.h"
#include "SimpleSprite.h"
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <Windows.h>
#include <glm/glm.hpp>
using glm::vec2;
using glm::vec3;
using namespace std;

struct SpriteDimensions {
	float width;
	float height;
	float adjustedWidth; // width after applying the multiplier
	float adjustedHeight; // height after applying the multiplier
};

namespace Helper {
	// For getting sprites and sprite sheets
	inline const char* PATH_TO_PLAYER_SPRITE_SHEET = ".\\Data\\SpriteSheets\\Player.bmp";
	inline const char* PATH_TO_ENEMY_SPRITE_SHEET = ".\\Data\\SpriteSheets\\Enemy.png";
	inline const char* PATH_TO_BULLET_SPRITE = ".\\Data\\Sprites\\Bullet.bmp";
	inline const char* PATH_TO_RELOADING_CIRCLE_SPRITE_SHEET = ".\\Data\\SpriteSheets\\ReloadingCircle.png";
	inline const char* PATH_TO_AMMO_EMPTY_SPRITE = ".\\Data\\Sprites\\AmmoEmpty.png";
	inline const char* PATH_TO_AMMO_FILLED_SPRITE = ".\\Data\\Sprites\\AmmoFilled.png";
	inline const char* PATH_TO_HEALTH_BAR_SPRITE_SHEET = ".\\Data\\SpriteSheets\\HealthBar.png";

	// Getting string representation of an EntityType (for reading Tag)
	inline string GetEntityTypeString(EntityType entityType) {
		switch (entityType) {
		case EntityType::Player:
			return "Player";
		case EntityType::Enemy:
			return "Enemy";
		case EntityType::Bullet:
			return "Bullet";
		default:
			return "Unknown";
		}
	}

	// Logging functions
	template<typename T>
	inline void Log(const string& message, T value) {
		stringstream ss;
		ss << message << value << "\n";
		OutputDebugStringA(ss.str().c_str());
	}

	inline void Log(const string& message) {
		OutputDebugStringA((message + "\n").c_str());
	}

	// Random value generation functions
	inline float GenerateFloat(float min, float max) {
		static random_device rd;
		static mt19937 eng(rd());
		uniform_real_distribution<> distr(min, max);
		return static_cast<float>(distr(eng));
	}

	inline vec2 GenerateVec2(float minX, float maxX, float minY, float maxY) {
		return vec2(GenerateFloat(minX, maxX), GenerateFloat(minY, maxY));
	}

	inline vec3 GenerateVec3(float minX, float maxX, float minY, float maxY, float minZ, float maxZ) {
		return vec3(GenerateFloat(minX, maxX), GenerateFloat(minY, maxY), GenerateFloat(minZ, maxZ));
	}

	// For spawning enemy in the opposite quadrant of player
	inline vec3 GetOppositeQuadrantPosition(const vec3& playerPos, float screenWidth, float screenHeight)
	{
		vec3 enemyPos;
		if (playerPos.x < screenWidth / 2)
			enemyPos.x = screenWidth * 0.75f; // Spawn in the right half
		else
			enemyPos.x = screenWidth * 0.25f; // Spawn in the left half
		if (playerPos.y < screenHeight / 2)
			enemyPos.y = screenHeight * 0.75f; // Spawn in the lower half
		else
			enemyPos.y = screenHeight * 0.25f; // Spawn in the upper half
		return enemyPos;
	}

	// For dealing with entities going out of bounds
	inline SpriteDimensions GetSpriteDimensions(shared_ptr<CSimpleSprite> sprite, float multiplier)
	{
		SpriteDimensions dimensions;
		if (sprite) {
			dimensions.width = sprite->GetWidth();
			dimensions.height = sprite->GetHeight();
			dimensions.adjustedWidth = dimensions.width * multiplier;
			dimensions.adjustedHeight = dimensions.height * multiplier;
		}
		else {
			dimensions.width = 0;
			dimensions.height = 0;
			dimensions.adjustedWidth = 0;
			dimensions.adjustedHeight = 0;
		}
		return dimensions;
	}
};