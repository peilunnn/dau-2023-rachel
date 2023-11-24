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
#include "App/Managers/include/SystemManager.h"
#include "App/Systems/include/InputHandler.h"
#include "App/Systems/include/MovementHandler.h"
#include "App/Systems/include/RenderingHandler.h"
#include "App/Systems/include/CollisionHandler.h"
#include "App/Systems/include/AnimationHandler.h"
#include "App/Utilities/include/Helper.h"
//------------------------------------------------------------------------

float screenWidth = 1024.0f;
float screenHeight = 768.0f;
glm::vec3 playerPos;
EntityManager entityManager;
SystemManager systemManager;
std::shared_ptr<CSimpleSprite> playerSprite;
std::shared_ptr<CSimpleSprite> enemySprite;
std::shared_ptr<CSimpleSprite> bulletSprite;
std::shared_ptr<CSimpleSprite> reloadingCircleSprite;
Entity playerEntity;
Entity enemyEntity;
Entity reloadingCircleEntity;
RenderingHandler renderingHandler;
InputHandler inputHandler;
MovementHandler movementHandler;
CollisionHandler collisionHandler;
AnimationHandler animationHandler;

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	CSimpleSprite* rawPlayerSprite = App::CreateSprite(".\\Data\\Sprites\\Player.bmp", 8, 4);
	playerSprite = std::shared_ptr<CSimpleSprite>(rawPlayerSprite);
	playerEntity = entityManager.CreatePlayerEntity(playerSprite);
	animationHandler.InitPlayerAnimation(playerSprite);

	CSimpleSprite* rawEnemySprite = App::CreateSprite(".\\Data\\Sprites\\Enemy.png", 4, 2);
	enemySprite = std::shared_ptr<CSimpleSprite>(rawEnemySprite);
	playerPos = entityManager.GetComponent<Transform>(playerEntity)->position;
	enemyEntity = entityManager.CreateEnemyEntity(entityManager, playerPos, enemySprite, screenWidth, screenHeight);
	animationHandler.InitEnemyAnimation(enemySprite);

	CSimpleSprite* rawBulletSprite = App::CreateSprite(".\\Data\\Sprites\\Bullet.bmp", 1, 1);
	bulletSprite = std::shared_ptr<CSimpleSprite>(rawBulletSprite);

	CSimpleSprite* rawReloadingCircleSprite = App::CreateSprite(".\\Data\\Sprites\\reloadingCircle.png", 5, 2);
	reloadingCircleSprite = std::shared_ptr<CSimpleSprite>(rawReloadingCircleSprite);
	reloadingCircleEntity = entityManager.CreateReloadingCircleEntity(reloadingCircleSprite);
	animationHandler.InitReloadingCircleAnimation(reloadingCircleSprite);

	systemManager.AddSystem(std::make_unique<AnimationHandler>());
	systemManager.AddSystem(std::make_unique<CollisionHandler>());
	systemManager.AddSystem(std::make_unique<InputHandler>());
	systemManager.AddSystem(std::make_unique<MovementHandler>());
	systemManager.AddSystem(std::make_unique<ShootingHandler>());
	systemManager.AddSystem(std::make_unique<RenderingHandler>());
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	inputHandler.Update(entityManager, deltaTime, playerEntity, bulletSprite);
	movementHandler.Update(entityManager, deltaTime, screenWidth, screenHeight);
	collisionHandler.Update(entityManager, systemManager, deltaTime);
	animationHandler.Update(entityManager, deltaTime);
	playerPos = entityManager.GetComponent<Transform>(playerEntity)->position;
	systemManager.ProcessEvents(entityManager, deltaTime, playerPos, screenWidth, screenHeight);
	entityManager.ProcessDeletions();
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.)
// See App.h
//------------------------------------------------------------------------
void Render()
{
	renderingHandler.Render(entityManager);
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
}