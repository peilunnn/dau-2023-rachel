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
#include "App/Systems/include/EntityHandler.h"
#include "App/Managers/include/SystemManager.h"
#include "App/Systems/include/AnimationHandler.h"
#include "App/Systems/include/CollisionHandler.h"
#include "App/Systems/include/CooldownHandler.h"
#include "App/Systems/include/InputHandler.h"
#include "App/Systems/include/MovementHandler.h"
#include "App/Systems/include/RenderingHandler.h"
#include "App/Systems/include/ShootingHandler.h"
#include "App/Systems/include/TimerHandler.h"
using namespace std;

//------------------------------------------------------------------------
GameState gameState;
CSimpleSprite* playerSprite;
CSimpleSprite* enemySprite;
CSimpleSprite* bulletSprite;
CSimpleSprite* reloadingCircleSprite;
CSimpleSprite* healthBarSprite;
vector<CSimpleSprite*> ammoEmptySprites;
vector<CSimpleSprite*> ammoFilledSprites;

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	gameState = GAMEPLAY;

	// Set up sprites
	playerSprite = App::CreateSprite(Helper::PATH_TO_PLAYER_SPRITE_SHEET, 8, 4);
	enemySprite = App::CreateSprite(Helper::PATH_TO_ENEMY_SPRITE_SHEET, 4, 2);
	bulletSprite = App::CreateSprite(Helper::PATH_TO_BULLET_SPRITE, 1, 1);
	reloadingCircleSprite = App::CreateSprite(Helper::PATH_TO_RELOADING_CIRCLE_SPRITE_SHEET, 5, 2);
	healthBarSprite = App::CreateSprite(Helper::PATH_TO_HEALTH_BAR_SPRITE_SHEET, 2, 3);
	const int maxBullets = ShootingHandler::GetInstance().MAX_BULLETS;
	constexpr int columns = 1;
	constexpr int rows = 1;

	for (int i = 0; i < maxBullets; ++i)
	{
		CSimpleSprite* ammoEmptySprite = App::CreateSprite(Helper::PATH_TO_AMMO_EMPTY_SPRITE, columns, rows);
		ammoEmptySprites.push_back(ammoEmptySprite);
		CSimpleSprite* ammoFilledSprite = App::CreateSprite(Helper::PATH_TO_AMMO_FILLED_SPRITE, columns, rows);
		ammoFilledSprites.push_back(ammoFilledSprite);
	}

	// Set up entities
	EntityManager& entityManager = EntityManager::GetInstance();
	entityManager.Init(playerSprite, enemySprite, reloadingCircleSprite, healthBarSprite, ammoEmptySprites, ammoFilledSprites);

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
	switch (gameState) {
	case MAIN_MENU:
		// Update and render main menu
		// Check for "Start" button click to change state to GAMEPLAY
		break;
	case GAMEPLAY:
		float deltaTimeInSeconds = deltaTime / 1000.0f;
		InputHandler::GetInstance().Update(deltaTimeInSeconds);
		MovementHandler::GetInstance().Update(deltaTimeInSeconds);
		CollisionHandler::GetInstance().Update(deltaTimeInSeconds);
		AnimationHandler::GetInstance().Update(deltaTimeInSeconds);
		TimerHandler::GetInstance().Update(deltaTimeInSeconds);
		CooldownHandler::GetInstance().Update(deltaTimeInSeconds);
		SystemManager::GetInstance().ProcessEvents(deltaTimeInSeconds);
		EntityManager::GetInstance().ProcessDeletions();
		break;
	}
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.)
// See App.h
//------------------------------------------------------------------------
void Render()
{
	RenderingHandler::GetInstance().Render(gameState);
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
	delete playerSprite;
	delete enemySprite;
	delete bulletSprite;
	delete reloadingCircleSprite;
	delete healthBarSprite;
	for (CSimpleSprite* sprite : ammoEmptySprites) {
		delete sprite;
	}
	ammoEmptySprites.clear();
	for (CSimpleSprite* sprite : ammoFilledSprites) {
		delete sprite;
	}
	ammoFilledSprites.clear();
}