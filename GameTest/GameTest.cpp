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
#include "App/Utilities/include/Helper.h"
#include "App/Managers/include/EntityManager.h"
#include "App/Managers/include/SystemManager.h"
#include "App/Managers/include/SpriteManager.h"
#include "App/Systems/include/AnimationHandler.h"
#include "App/Systems/include/InputHandler.h"
#include "App/Systems/include/MovementHandler.h"
#include "App/Systems/include/RenderingHandler.h"
#include "App/Systems/include/CollisionHandler.h"
#include "App/Systems/include/AnimationHandler.h"
#include "App/Systems/include/TimerHandler.h"
using namespace std;

//------------------------------------------------------------------------

vec3 playerPos;
shared_ptr<CSimpleSprite> playerSprite;
shared_ptr<CSimpleSprite> enemySprite;
shared_ptr<CSimpleSprite> bulletSprite;
shared_ptr<CSimpleSprite> reloadingCircleSprite;
shared_ptr<CSimpleSprite> ammoEmptySprite;
shared_ptr<CSimpleSprite> ammoFilledSprite;
shared_ptr<CSimpleSprite> healthBarSprite;
EntityId playerEntityId;
EntityId enemyEntityId;
EntityId reloadingCircleEntityId;
EntityId ammoEmptyEntityId;
EntityId ammoFilledEntityId;
EntityId healthBarEntityId;
EntityId scoreEntityId;
EntityId timerEntityId;

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	// Set up sprites
	CSimpleSprite* rawPlayerSprite = App::CreateSprite(Helper::PATH_TO_PLAYER_SPRITE_SHEET, 8, 4);
	playerSprite = shared_ptr<CSimpleSprite>(rawPlayerSprite);
	CSimpleSprite* rawEnemySprite = App::CreateSprite(Helper::PATH_TO_ENEMY_SPRITE_SHEET, 4, 2);
	enemySprite = shared_ptr<CSimpleSprite>(rawEnemySprite);
	CSimpleSprite* rawBulletSprite = App::CreateSprite(Helper::PATH_TO_BULLET_SPRITE, 1, 1);
	bulletSprite = shared_ptr<CSimpleSprite>(rawBulletSprite);
	CSimpleSprite* rawReloadingCircleSprite = App::CreateSprite(Helper::PATH_TO_RELOADING_CIRCLE_SPRITE_SHEET, 5, 2);
	reloadingCircleSprite = shared_ptr<CSimpleSprite>(rawReloadingCircleSprite);
	CSimpleSprite* rawHealthBarSprite = App::CreateSprite(Helper::PATH_TO_HEALTH_BAR_SPRITE_SHEET, 2, 3);
	healthBarSprite = shared_ptr<CSimpleSprite>(rawHealthBarSprite);

	// Set up entities
	EntityManager& entityManager = EntityManager::GetInstance();
	entityManager.Init(playerSprite, enemySprite, reloadingCircleSprite, ammoEmptySprite, ammoFilledSprite, healthBarSprite);
	playerEntityId = entityManager.GetPlayerEntityId();
	enemyEntityId = entityManager.GetEnemyEntityId();
	reloadingCircleEntityId = entityManager.GetReloadingCircleEntityId();
	healthBarEntityId = entityManager.GetHealthBarEntityId();
	scoreEntityId = entityManager.GetScoreEntityId();
	timerEntityId = entityManager.GetTimerEntityId();

	// Set up managers and systems
	SystemManager::GetInstance().Init();

	// Set up animations
	AnimationHandler::GetInstance().Init(playerSprite, enemySprite, reloadingCircleSprite, healthBarSprite);
	
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	float deltaTimeInSeconds = deltaTime / 1000.0f;
	InputHandler::GetInstance().Update(deltaTimeInSeconds, playerEntityId, bulletSprite);
	MovementHandler::GetInstance().Update(deltaTimeInSeconds);
	CollisionHandler::GetInstance().Update(deltaTimeInSeconds);
	AnimationHandler::GetInstance().Update(deltaTimeInSeconds);
	SystemManager::GetInstance().ProcessEvents(deltaTimeInSeconds);
	EntityManager::GetInstance().ProcessDeletions();
	TimerHandler::GetInstance().Update(deltaTimeInSeconds);
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.)
// See App.h
//------------------------------------------------------------------------
void Render()
{
	RenderingHandler::GetInstance().Render(EntityManager::GetInstance());
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
}