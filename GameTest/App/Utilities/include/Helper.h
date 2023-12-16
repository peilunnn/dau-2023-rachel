#pragma once
#include "Components/include/EntityId.h"
#include "Components/include/Renderable.h"
#include "Components/include/Screen.h"
#include "Components/include/Tag.h"
#include "Components/include/Transform.h"
#include "Managers/include/EntityManager.h"
#include "Utilities/include/app.h"
#include "Utilities/include/Enums.h"
#include <glm/glm.hpp>
#include <random>
#include <string>
#include <Windows.h>
#include <sstream>
using glm::vec2;
using glm::vec3;
using namespace std;

namespace Helper
{
	// Sprites
	inline const char *PATH_TO_PLAYER = ".\\Data\\SpriteSheets\\Player.png";
	inline const char *PATH_TO_ENEMY = ".\\Data\\SpriteSheets\\Enemy.png";
	inline const char *PATH_TO_BULLET_SPRITE = ".\\Data\\Sprites\\Bullet.bmp";
	inline const char *PATH_TO_AMMO_PICKUP = ".\\Data\\Sprites\\AmmoPickup.png";
	inline const char *PATH_TO_HEALTH_PICKUP = ".\\Data\\Sprites\\HealthPickup.png";
	inline const char *PATH_TO_LIGHTNING_PICKUP = ".\\Data\\Sprites\\LightningPickup.png";
	inline const char *PATH_TO_AMMO_EMPTY = ".\\Data\\Sprites\\AmmoEmpty.png";
	inline const char *PATH_TO_AMMO_FILLED = ".\\Data\\Sprites\\AmmoFilled.png";
	inline const char *PATH_TO_HEALTH_BAR = ".\\Data\\SpriteSheets\\HealthBar.png";
	inline const char *PATH_TO_TITLE = ".\\Data\\Sprites\\Title.png";
	inline const char *PATH_TO_PLAY_BUTTON = ".\\Data\\Sprites\\PlayButton.png";
	inline const char *PATH_TO_BACK_BUTTON = ".\\Data\\Sprites\\BackButton.png";
	inline const char *PATH_TO_QUIT_BUTTON = ".\\Data\\Sprites\\QuitButton.png";
	inline const char *PATH_TO_STARFIELD = ".\\Data\\Sprites\\Starfield.png";
	inline const char *PATH_TO_CROSSHAIR = ".\\Data\\Sprites\\Crosshair.png";
	inline const char *PATH_TO_LIGHTNING_STRIKE = ".\\Data\\SpriteSheets\\LightningStrike.png";

	// Sounds
	inline const char *PATH_TO_TEST = ".\\Data\\Sounds\\Test.wav";
	inline const char *PATH_TO_GUNFIRE = ".\\Data\\Sounds\\Gunfire.wav";
	inline const char* PATH_TO_RELOAD = ".\\Data\\Sounds\\Reload.wav";
	inline const char* PATH_TO_HEALTH_BOOST = ".\\Data\\Sounds\\HealthBoost.wav";
	inline const char *PATH_TO_THUNDER = ".\\Data\\Sounds\\Thunder.wav";
	inline const char *PATH_TO_HURT = ".\\Data\\Sounds\\Hurt.wav";
	inline const char *PATH_TO_BUTTON_CLICK = ".\\Data\\Sounds\\ButtonClick.wav";
	inline const char *PATH_TO_GAMEPLAY_MUSIC = ".\\Data\\Sounds\\GameplayMusic.wav";
	inline const char *PATH_TO_NON_GAMEPLAY_MUSIC = ".\\Data\\Sounds\\NonGameplayMusic.wav";

	template <typename T>
	inline void Log(const string &message, T value)
	{
		stringstream ss;
		ss << message << value << "\n";
		OutputDebugStringA(ss.str().c_str());
	}

	inline void Log(const string &message)
	{
		OutputDebugStringA((message + "\n").c_str());
	}

	inline float GenerateFloat(float min, float max)
	{
		srand(static_cast<unsigned int>(time(nullptr)));

		static random_device rd;
		static mt19937 eng(rd());
		uniform_real_distribution<> distr(min, max);
		return static_cast<float>(distr(eng));
	}

	inline vec2 GenerateVec2(float minX, float maxX, float minY, float maxY)
	{
		return vec2(GenerateFloat(minX, maxX), GenerateFloat(minY, maxY));
	}

	inline vec3 GetOppositeQuadrantPosition(const vec3 &playerPos, float borderWidth, float borderHeight)
	{
		vec3 enemyPos;
		float spawnOffsetMajor = 0.7f;
		float spawnOffsetMinor = 0.3f;
		float xMidpoint = borderWidth / 2.0f;
		float yMidpoint = borderHeight / 2.0f;

		if (playerPos.x < xMidpoint)
			enemyPos.x = borderWidth * spawnOffsetMajor; // Spawn in the right half
		else
			enemyPos.x = borderWidth * spawnOffsetMinor; // Spawn in the left half

		if (playerPos.y < yMidpoint)
			enemyPos.y = borderHeight * spawnOffsetMajor; // Spawn in the lower half
		else
			enemyPos.y = borderHeight * spawnOffsetMinor; // Spawn in the upper half

		enemyPos.z = 0.0f;

		return enemyPos;
	}
};