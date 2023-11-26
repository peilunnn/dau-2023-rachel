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
#include "App/Utilities/include/Helper.h"
#include "App/Systems/include/AnimationHandler.h"
#include "App/Systems/include/InputHandler.h"
#include "App/Systems/include/MovementHandler.h"
#include "App/Systems/include/RenderingHandler.h"
#include "App/Systems/include/CollisionHandler.h"
#include "App/Systems/include/AnimationHandler.h"

//------------------------------------------------------------------------

int maxBullets;
glm::vec3 playerPos;
EntityManager entityManager;
SystemManager systemManager;
std::shared_ptr<CSimpleSprite> playerSprite;
std::shared_ptr<CSimpleSprite> enemySprite;
std::shared_ptr<CSimpleSprite> bulletSprite;
std::shared_ptr<CSimpleSprite> reloadingCircleSprite;
std::shared_ptr<CSimpleSprite> ammoEmptySprite;
std::shared_ptr<CSimpleSprite> ammoFilledSprite;
std::shared_ptr<CSimpleSprite> healthBarSprite;
Entity playerEntity;
Entity enemyEntity;
Entity reloadingCircleEntity;
Entity ammoEmptyEntity;
Entity ammoFilledEntity;
Entity healthBarEntity;
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
	// Set up sprites
	CSimpleSprite* rawPlayerSprite = App::CreateSprite(Helper::PATH_TO_PLAYER_SPRITE_SHEET, 8, 4);
	playerSprite = std::shared_ptr<CSimpleSprite>(rawPlayerSprite);
	CSimpleSprite* rawEnemySprite = App::CreateSprite(Helper::PATH_TO_ENEMY_SPRITE_SHEET, 4, 2);
	enemySprite = std::shared_ptr<CSimpleSprite>(rawEnemySprite);
	CSimpleSprite* rawBulletSprite = App::CreateSprite(Helper::PATH_TO_BULLET_SPRITE, 1, 1);
	bulletSprite = std::shared_ptr<CSimpleSprite>(rawBulletSprite);
	CSimpleSprite* rawReloadingCircleSprite = App::CreateSprite(Helper::PATH_TO_RELOADING_CIRCLE_SPRITE_SHEET, 5, 2);
	reloadingCircleSprite = std::shared_ptr<CSimpleSprite>(rawReloadingCircleSprite);
	CSimpleSprite* rawHealthBarSprite = App::CreateSprite(Helper::PATH_TO_HEALTH_BAR_SPRITE_SHEET, 2, 3);
	healthBarSprite = std::shared_ptr<CSimpleSprite>(rawHealthBarSprite);

	// Set up entities
	entityManager.Init(playerSprite, enemySprite, reloadingCircleSprite, ammoEmptySprite, ammoFilledSprite, healthBarSprite);
	playerEntity = entityManager.GetPlayerEntity();
	enemyEntity = entityManager.GetEnemyEntity();
	reloadingCircleEntity = entityManager.GetReloadingCircleEntity();
	healthBarEntity = entityManager.GetHealthBarEntity();

	// Set up animations
	animationHandler.Init(playerSprite, enemySprite, reloadingCircleSprite, healthBarSprite);
	
	// Set up systems
	systemManager.Init();
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	float deltaTimeInSeconds = deltaTime / 1000.0f;
	inputHandler.Update(entityManager, deltaTimeInSeconds, playerEntity, bulletSprite);
	movementHandler.Update(entityManager, deltaTimeInSeconds);
	collisionHandler.Update(entityManager, systemManager, deltaTimeInSeconds);
	animationHandler.Update(entityManager, deltaTimeInSeconds);
	playerPos = entityManager.GetComponent<Transform>(playerEntity)->position;
	systemManager.ProcessEvents(entityManager, deltaTimeInSeconds, playerPos);
	entityManager.ProcessDeletions();
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.)
// See App.h
//------------------------------------------------------------------------
void Render()
{
	renderingHandler.SetBackground(0.2f, 0.2f, 0.2f, 1.0f);
	renderingHandler.DrawBorder(1.0f, 1.0f, 1.0f);
	renderingHandler.DrawBackgroundInBorder(0.0f, 0.0f, 0.0f);
	renderingHandler.Render(entityManager);
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
}