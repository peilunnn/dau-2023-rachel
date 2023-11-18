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
#include "App/Utilities/include/GenerateRandomValue.h"
#include "App/Managers/include/EntityManager.h"
#include "App/Systems/include/HandleInput.h"
#include "App/Systems/include/HandlePlayerMovement.h"
#include "App/Systems/include/HandleEnemyMovement.h"
#include "App/Systems/include/RenderEntities.h"
#include "App/Systems/include/HandleAnimation.h"
#include "App/Utilities/include/Debug.h"
//------------------------------------------------------------------------

float screenWidth = 1024.0f;
float screenHeight = 768.0f;
EntityManager entityManager;
Entity playerEntityId;
Entity enemyEntityId;
HandleInput handleInput;
HandlePlayerMovement handlePlayerMovement;
HandleEnemyMovement handleEnemyMovement;
RenderEntities renderEntities;
CSimpleSprite* playerSprite;
CSimpleSprite *enemySprite;
HandleAnimation handleAnimation;

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	playerSprite = App::CreateSprite(".\\Data\\SpriteSheets\\Player.bmp", 8, 4);
	playerEntityId = entityManager.CreatePlayerEntity(playerSprite);
	handleAnimation.Init(playerSprite);

	glm::vec3 playerPos = entityManager.GetComponent<Transform>(playerEntityId)->position;
	enemySprite = App::CreateSprite(".\\Data\\SpriteSheets\\EnemyTest.png", 1, 1);
	enemyEntityId = entityManager.CreateEnemyEntity(entityManager, playerPos, enemySprite, screenWidth, screenHeight);
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	playerSprite->Update(deltaTime);
	handleInput.Update(entityManager, deltaTime, playerEntityId);
	handlePlayerMovement.Update(entityManager, deltaTime);
	handleEnemyMovement.Update(entityManager, deltaTime, screenWidth, screenHeight);
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