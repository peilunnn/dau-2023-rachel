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
#include "Utilities/include/app.h"
#include "Utilities/include/Helper.h"
#include "Managers/include/EntityManager.h"
#include "Systems/include/EntityHandler.h"
#include "Managers/include/SystemManager.h"
#include "Systems/include/AnimationHandler.h"
#include "Systems/include/CollisionHandler.h"
#include "Systems/include/CooldownHandler.h"
#include "Systems/include/InputHandler.h"
#include "Systems/include/MovementHandler.h"
#include "Systems/include/RenderingHandler.h"
#include "Systems/include/ShootingHandler.h"
#include "Systems/include/TimerHandler.h"
#include "Systems/include/TitleHandler.h"
using namespace std;

//------------------------------------------------------------------------
GameState gameState;
CSimpleSprite* playerSprite;
CSimpleSprite* enemySprite;
CSimpleSprite* bulletSprite;
CSimpleSprite* reloadingCircleSprite;
CSimpleSprite* healthBarSprite;
CSimpleSprite* titleSprite;
CSimpleSprite* playButtonSprite;
vector<CSimpleSprite*> ammoEmptySprites;
vector<CSimpleSprite*> ammoFilledSprites;

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	gameState = GAMEPLAY;

	// Set up sprites
	// Main Menu
	titleSprite = App::CreateSprite(Helper::PATH_TO_TITLE, 1, 1);
	playButtonSprite = App::CreateSprite(Helper::PATH_TO_PLAY_BUTTON, 1, 1);

	// Gameplay
	playerSprite = App::CreateSprite(Helper::PATH_TO_PLAYER, 4, 4);
	enemySprite = App::CreateSprite(Helper::PATH_TO_ENEMY, 4, 2);
	bulletSprite = App::CreateSprite(Helper::PATH_TO_BULLET_SPRITE, 1, 1);
	reloadingCircleSprite = App::CreateSprite(Helper::PATH_TO_RELOADING_CIRCLE, 5, 2);
	healthBarSprite = App::CreateSprite(Helper::PATH_TO_HEALTH_BAR, 2, 3);
	const int maxBullets = ShootingHandler::GetInstance().MAX_BULLETS;
	constexpr int columns = 1;
	constexpr int rows = 1;

	for (int i = 0; i < maxBullets; ++i)
	{
		CSimpleSprite* ammoEmptySprite = App::CreateSprite(Helper::PATH_TO_AMMO_EMPTY, columns, rows);
		ammoEmptySprites.push_back(ammoEmptySprite);
		CSimpleSprite* ammoFilledSprite = App::CreateSprite(Helper::PATH_TO_AMMO_FILLED, columns, rows);
		ammoFilledSprites.push_back(ammoFilledSprite);
	}

	// Set up entities
	EntityManager& entityManager = EntityManager::GetInstance();
	entityManager.Init(playerSprite, enemySprite, reloadingCircleSprite, healthBarSprite, titleSprite, playButtonSprite, ammoEmptySprites, ammoFilledSprites);

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

	switch (gameState) {
	case MAIN_MENU:
		// Update and render main menu
		TitleHandler::GetInstance().Update(deltaTimeInSeconds);
		// Check for "Start" button click to change state to GAMEPLAY
		break;
	case GAMEPLAY:
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
	RenderingHandler::GetInstance().Render(gameState, titleSprite, playButtonSprite);
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
	delete titleSprite;
	delete playButtonSprite;
	for (CSimpleSprite* sprite : ammoEmptySprites) {
		delete sprite;
	}
	ammoEmptySprites.clear();
	for (CSimpleSprite* sprite : ammoFilledSprites) {
		delete sprite;
	}
	ammoFilledSprites.clear();
}