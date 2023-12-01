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
#include "App/Managers/include/SpriteManager.h"
#include "App/Utilities/include/Helper.h"
#include "App/Systems/include/AnimationHandler.h"
#include "App/Systems/include/InputHandler.h"
#include "App/Systems/include/MovementHandler.h"
#include "App/Systems/include/RenderingHandler.h"
#include "App/Systems/include/CollisionHandler.h"
#include "App/Systems/include/AnimationHandler.h"
#include "App/Systems/include/TimerHandler.h"
using namespace std;

//------------------------------------------------------------------------

glm::vec3 playerPos;
EntityManager entityManager;
SystemManager systemManager;
shared_ptr<CSimpleSprite> playerSprite;
shared_ptr<CSimpleSprite> enemySprite;
shared_ptr<CSimpleSprite> bulletSprite;
shared_ptr<CSimpleSprite> reloadingCircleSprite;
shared_ptr<CSimpleSprite> ammoEmptySprite;
shared_ptr<CSimpleSprite> ammoFilledSprite;
shared_ptr<CSimpleSprite> healthBarSprite;
EntityId playerEntityId;
EntityId enemyEntity;
EntityId reloadingCircleEntity;
EntityId ammoEmptyEntity;
EntityId ammoFilledEntity;
EntityId healthBarEntity;
EntityId scoreEntity;
EntityId timerEntity;
RenderingHandler renderingHandler;
InputHandler inputHandler;
MovementHandler movementHandler;
CollisionHandler collisionHandler;
AnimationHandler animationHandler;
ScoreHandler scoreHandler;
TimerHandler timerHandler;

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

	// playerSprite = SpriteManager::CreateSprite(Helper::PATH_TO_PLAYER_SPRITE_SHEET, 8, 4);
	// enemySprite = SpriteManager::CreateSprite(Helper::PATH_TO_ENEMY_SPRITE_SHEET, 4, 2);
	// bulletSprite = SpriteManager::CreateSprite(Helper::PATH_TO_BULLET_SPRITE, 1, 1);
	// reloadingCircleSprite = SpriteManager::CreateSprite(Helper::PATH_TO_RELOADING_CIRCLE_SPRITE_SHEET, 5, 2);
	// healthBarSprite = SpriteManager::CreateSprite(Helper::PATH_TO_HEALTH_BAR_SPRITE_SHEET, 2, 3);

	// Set up entities
	entityManager.Init(playerSprite, enemySprite, reloadingCircleSprite, ammoEmptySprite, ammoFilledSprite, healthBarSprite);
	playerEntityId = entityManager.GetPlayerEntityId();
	enemyEntity = entityManager.GetEnemyEntityId();
	reloadingCircleEntity = entityManager.GetReloadingCircleEntityId();
	healthBarEntity = entityManager.GetHealthBarEntityId();
	scoreEntity = entityManager.GetScoreEntityId();
	timerEntity = entityManager.GetTimerEntityId();

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
	inputHandler.Update(entityManager, deltaTimeInSeconds, playerEntityId, bulletSprite);
	movementHandler.Update(entityManager, deltaTimeInSeconds);
	collisionHandler.Update(entityManager, systemManager, deltaTimeInSeconds);
	animationHandler.Update(entityManager, deltaTimeInSeconds);
	playerPos = entityManager.GetComponent<Transform>(playerEntityId)->GetPosition();
	systemManager.ProcessEvents(entityManager, scoreHandler, deltaTimeInSeconds, playerPos);
	entityManager.ProcessDeletions();
	timerHandler.Update(entityManager, deltaTimeInSeconds);
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