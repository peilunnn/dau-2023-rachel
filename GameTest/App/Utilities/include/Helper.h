#pragma once
#include "Components/include/EntityId.h"
#include "Components/include/Renderable.h"
#include "Components/include/Tag.h"
#include "Components/include/Transform.h"
#include "Managers/include/EntityManager.h"
#include "Utilities/include/app.h"
#include "Utilities/include/Enums.h"
#include <glm/glm.hpp>
#include <random>
#include <string>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")
using glm::vec2;
using glm::vec3;
using namespace std;

namespace Helper
{
	// Sprites
	inline const char* PATH_TO_PLAYER = ".\\Data\\SpriteSheets\\Player.png";
	inline const char* PATH_TO_ENEMY = ".\\Data\\SpriteSheets\\Enemy.png";
	inline const char* PATH_TO_BULLET_SPRITE = ".\\Data\\Sprites\\Bullet.bmp";
	inline const char* PATH_TO_RELOADING_CIRCLE = ".\\Data\\SpriteSheets\\ReloadingCircle.png";
	inline const char* PATH_TO_AMMO_EMPTY = ".\\Data\\Sprites\\AmmoEmpty.png";
	inline const char* PATH_TO_AMMO_FILLED = ".\\Data\\Sprites\\AmmoFilled.png";
	inline const char* PATH_TO_HEALTH_BAR = ".\\Data\\SpriteSheets\\HealthBar.png";
	inline const char* PATH_TO_TITLE = ".\\Data\\Sprites\\Title.png";
	inline const char* PATH_TO_PLAY_BUTTON = ".\\Data\\Sprites\\PlayButton.png";
	inline const char* PATH_TO_BACK_BUTTON = ".\\Data\\Sprites\\BackButton.png";
	inline const char* PATH_TO_STARFIELD = ".\\Data\\Sprites\\Starfield.png";

	// Sounds
	inline const char* PATH_TO_GUNFIRE = ".\\Data\\Sounds\\Gunfire.wav";
	inline const char* PATH_TO_RELOAD = ".\\Data\\Sounds\\Reload.wav";
	inline const char* PATH_TO_HURT = ".\\Data\\Sounds\\Hurt.wav";
	inline const char* PATH_TO_BUTTON_CLICK = ".\\Data\\Sounds\\ButtonClick.wav";
	inline const char* PATH_TO_GAMEPLAY_MUSIC = ".\\Data\\Sounds\\GameplayMusic.wav";
	inline const char* PATH_TO_NON_GAMEPLAY_MUSIC = ".\\Data\\Sounds\\NonGameplayMusic.wav";

	inline void PlaySoundFromFile(const char* soundFilePath, bool loop = false)
	{
		DWORD flags = SND_FILENAME | SND_ASYNC;
		if (loop) {
			flags |= SND_LOOP;
		}
		PlaySoundA(soundFilePath, NULL, flags);
	}

	inline void StopSound()
	{
		PlaySoundA(NULL, NULL, 0);
	}

	template <typename T>
	inline void Log(const string& message, T value)
	{
		stringstream ss;
		ss << message << value << "\n";
		OutputDebugStringA(ss.str().c_str());
	}

	inline void Log(const string& message)
	{
		OutputDebugStringA((message + "\n").c_str());
	}

	inline float GenerateFloat(float min, float max)
	{
		static random_device rd;
		static mt19937 eng(rd());
		uniform_real_distribution<> distr(min, max);
		return static_cast<float>(distr(eng));
	}

	inline vec2 GenerateVec2(float minX, float maxX, float minY, float maxY)
	{
		return vec2(GenerateFloat(minX, maxX), GenerateFloat(minY, maxY));
	}

	inline vec3 GenerateVec3(float minX, float maxX, float minY, float maxY, float minZ, float maxZ)
	{
		return vec3(GenerateFloat(minX, maxX), GenerateFloat(minY, maxY), GenerateFloat(minZ, maxZ));
	}

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

	inline bool IsButtonClicked(EntityId entityId)
	{
		EntityManager& entityManager = EntityManager::GetInstance();
		Transform* transform = entityManager.GetComponent<Transform>(entityId);
		Renderable* renderable = entityManager.GetComponent<Renderable>(entityId);
		CSimpleSprite* sprite = renderable->GetSprite();

		float scale = sprite->GetScale();
		const float actualWidth = sprite->GetWidth() * scale;
		const float actualHeight = sprite->GetHeight() * scale;

		float mouseX, mouseY;
		App::GetMousePos(mouseX, mouseY);

		float left = transform->GetPosition().x - actualWidth / 2.0f;
		float right = transform->GetPosition().x + actualWidth / 2.0f;
		float top = transform->GetPosition().y - actualHeight / 2.0f;
		float bottom = transform->GetPosition().y + actualHeight / 2.0f;

		bool isWithinX = mouseX >= left && mouseX <= right;
		bool isWithinY = mouseY >= top && mouseY <= bottom;

		return isWithinX && isWithinY && App::IsKeyPressed(VK_LBUTTON);
	}
};