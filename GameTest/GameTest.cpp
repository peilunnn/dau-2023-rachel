//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h>
#include <math.h>
#include <memory>
#include <iostream>
//------------------------------------------------------------------------
#include "App/Utilities/include/app.h"
#include "App/Managers/include/EntityManager.h"
#include "App/Systems/include/HandleInput.h"
#include "App/Systems/include/HandleMovement.h"
#include "App/Systems/include/RenderEntities.h"
#include "App/Systems/include/HandleAnimation.h"
#include "App/Utilities/include/Helper.h"
//------------------------------------------------------------------------

float screenWidth = 1024.0f;
float screenHeight = 768.0f;
EntityManager entityManager;
Entity playerEntityId;
Entity enemyEntityId;
HandleInput handleInput;
HandleMovement handleMovement;
RenderEntities renderEntities;
CSimpleSprite* playerSprite;
CSimpleSprite* enemySprite;
CSimpleSprite* bulletSprite;
HandleAnimation handleAnimation;

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	playerSprite = App::CreateSprite(".\\Data\\Sprites\\Player.bmp", 8, 4);
	playerEntityId = entityManager.CreatePlayerEntity(playerSprite);
	handleAnimation.InitPlayerAnimation(playerSprite);

	enemySprite = App::CreateSprite(".\\Data\\Sprites\\Enemy.png", 4, 2);
	glm::vec3 playerPos = entityManager.GetComponent<Transform>(playerEntityId)->position;
	enemyEntityId = entityManager.CreateEnemyEntity(entityManager, playerPos, enemySprite, screenWidth, screenHeight);
	handleAnimation.InitEnemyAnimation(enemySprite);

	bulletSprite = App::CreateSprite(".\\Data\\Sprites\\Bullet.bmp", 1, 1);
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	handleInput.Update(entityManager, deltaTime, playerEntityId, bulletSprite);
	handleMovement.Update(entityManager, deltaTime, screenWidth, screenHeight);
	handleAnimation.Update(entityManager, deltaTime);
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.)
// See App.h
//------------------------------------------------------------------------
void Render()
{
	renderEntities.Render(entityManager);
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
	delete playerSprite;
	delete enemySprite;
}