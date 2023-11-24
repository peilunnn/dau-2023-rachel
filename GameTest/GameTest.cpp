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
std::shared_ptr<CSimpleSprite> ammoEmptySprite;
std::shared_ptr<CSimpleSprite> ammoFilledSprite;
std::vector<Entity> ammoEmptyEntities;
std::vector<Entity> ammoFilledEntities;
Entity playerEntity;
Entity enemyEntity;
Entity reloadingCircleEntity;
Entity ammoEmptyEntity;
Entity ammoFilledEntity;
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
	CSimpleSprite *rawPlayerSprite = App::CreateSprite(Helper::pathToPlayerSpriteSheet, 8, 4);
	playerSprite = std::shared_ptr<CSimpleSprite>(rawPlayerSprite);
	playerEntity = entityManager.CreatePlayerEntity(playerSprite);
	animationHandler.InitPlayerAnimation(playerSprite);

	CSimpleSprite *rawEnemySprite = App::CreateSprite(Helper::pathToEnemySpriteSheet, 4, 2);
	enemySprite = std::shared_ptr<CSimpleSprite>(rawEnemySprite);
	playerPos = entityManager.GetComponent<Transform>(playerEntity)->position;
	enemyEntity = entityManager.CreateEnemyEntity(entityManager, playerPos, enemySprite, screenWidth, screenHeight);
	animationHandler.InitEnemyAnimation(enemySprite);

	CSimpleSprite *rawBulletSprite = App::CreateSprite(Helper::pathToBulletSprite, 1, 1);
	bulletSprite = std::shared_ptr<CSimpleSprite>(rawBulletSprite);

	CSimpleSprite *rawReloadingCircleSprite = App::CreateSprite(Helper::pathToReloadingCircleSpriteSheet, 5, 2);
	reloadingCircleSprite = std::shared_ptr<CSimpleSprite>(rawReloadingCircleSprite);
	reloadingCircleEntity = entityManager.CreateReloadingCircleEntity(reloadingCircleSprite);
	animationHandler.InitReloadingCircleAnimation(reloadingCircleSprite);

	float startingX = screenWidth - 20;
	float yPos = screenHeight - 40;
	float ammoSpriteSpacing = 30;
	int maxBullets = ShootingHandler::GetMaxBullets();
	for (int i = 0; i < maxBullets; ++i)
	{
		float xPos = startingX - i * ammoSpriteSpacing;

		CSimpleSprite *rawAmmoEmptySprite = App::CreateSprite(Helper::pathToAmmoEmptySprite, 1, 1);
		ammoEmptySprite = std::shared_ptr<CSimpleSprite>(rawAmmoEmptySprite);
		ammoEmptyEntity = entityManager.CreateAmmoEntity(ammoEmptySprite, EntityType::AMMO_EMPTY, xPos, yPos);
		ammoEmptyEntities.push_back(ammoEmptyEntity);

		CSimpleSprite *rawAmmoFilledSprite = App::CreateSprite(Helper::pathToAmmoFilledSprite, 1, 1);
		ammoFilledSprite = std::shared_ptr<CSimpleSprite>(rawAmmoFilledSprite);
		ammoFilledEntity = entityManager.CreateAmmoEntity(ammoFilledSprite, EntityType::AMMO_FILLED, xPos, yPos);
		ammoFilledEntities.push_back(ammoFilledEntity);
	}
	
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
	float deltaTimeInSeconds = deltaTime / 1000.0f;
	inputHandler.Update(entityManager, deltaTimeInSeconds, playerEntity, bulletSprite);
	movementHandler.Update(entityManager, deltaTimeInSeconds, screenWidth, screenHeight);
	collisionHandler.Update(entityManager, systemManager, deltaTimeInSeconds);
	animationHandler.Update(entityManager, deltaTimeInSeconds);
	playerPos = entityManager.GetComponent<Transform>(playerEntity)->position;
	systemManager.ProcessEvents(entityManager, deltaTimeInSeconds, playerPos, screenWidth, screenHeight);
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