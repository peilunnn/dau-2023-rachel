//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
//------------------------------------------------------------------------
#include "Managers/include/EntityManager.h"
#include "Managers/include/SystemManager.h"
#include "Managers/include/SpriteManager.h"
#include "Systems/include/AnimationHandler.h"
#include "Systems/include/CollisionHandler.h"
#include "Systems/include/CooldownHandler.h"
#include "Systems/include/InputHandler.h"
#include "Systems/include/MovementHandler.h"
#include "Systems/include/RenderingHandler.h"
#include "Systems/include/TimerHandler.h"
#include "Systems/include/TitleHandler.h"
using namespace std;

//------------------------------------------------------------------------

GameState gameState;

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	// Set game state
	gameState = GAMEPLAY;

	// Set up sprites
	SpriteManager& spriteManager = SpriteManager::GetInstance();

	// Set up entities
	EntityManager& entityManager = EntityManager::GetInstance();
	entityManager.Init();

	// Set up managers and systems
	SystemManager::GetInstance().Init();

	// Set up animations
	AnimationHandler::GetInstance().Init();
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
	RenderingHandler::GetInstance().Render(gameState);
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
	SpriteManager::GetInstance().DestroyAllSprites();
}