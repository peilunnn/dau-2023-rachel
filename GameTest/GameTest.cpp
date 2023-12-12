//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
//------------------------------------------------------------------------
#include "Managers/include/EntityManager.h"
#include "Managers/include/GameManager.h"
#include "Managers/include/SystemManager.h"
#include "Managers/include/SpriteManager.h"
#include "Managers/include/SoundManager.h"
#include "Systems/include/AnimationHandler.h"
#include "Systems/include/CollisionHandler.h"
#include "Systems/include/CooldownHandler.h"
#include "Systems/include/InputHandler.h"
#include "Systems/include/MovementHandler.h"
#include "Systems/include/RenderingHandler.h"
#include "Systems/include/TimerHandler.h"
#include "Systems/include/TitleHandler.h"
#include "Utilities/include/Helper.h"
using namespace std;

//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	// Seed the random number generator
	srand(static_cast<unsigned int>(time(nullptr)));
	
	// Set up entities
	EntityManager &entityManager = EntityManager::GetInstance();
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
	GameManager& gameManager = GameManager::GetInstance();
	float deltaTimeInSeconds = deltaTime / 1000.0f;
	gameManager.Update(deltaTimeInSeconds);

	if (gameManager.GetCurrentGameState() == GameState::MainMenu)
	{
		if (!gameManager.GetGameReset())
		{
			gameManager.ResetGame();
			SoundManager::GetInstance().PlaySoundFromFile(Helper::PATH_TO_NON_GAMEPLAY_MUSIC, true);
			gameManager.SetGameReset(true);
		}
		TitleHandler::GetInstance().OscillateTitle(deltaTimeInSeconds);
		InputHandler::GetInstance().SetIsPlayButtonClicked();
		gameManager.HandlePlayButtonClick();
	}
	else if (gameManager.GetCurrentGameState() == GameState::Gameplay)
	{
		InputHandler::GetInstance().Update(deltaTimeInSeconds);
		MovementHandler::GetInstance().Update(deltaTimeInSeconds);
		CollisionHandler::GetInstance().Update(deltaTimeInSeconds);
		RenderingHandler::GetInstance().UpdateScreenShakeTimer(deltaTime);
		AnimationHandler::GetInstance().Update(deltaTimeInSeconds);
		TimerHandler::GetInstance().Update(deltaTimeInSeconds);
		CooldownHandler::GetInstance().Update(deltaTimeInSeconds);
		SystemManager::GetInstance().ProcessEvents(deltaTimeInSeconds);
		EntityManager::GetInstance().ProcessDeletions();
	}
	else if (gameManager.GetCurrentGameState() == GameState::GameOver)
	{
		gameManager.SetGameReset(false);
		InputHandler::GetInstance().SetIsBackButtonClicked();
		gameManager.HandleBackButtonClick();
	}
	else if (gameManager.GetCurrentGameState() == GameState::Loading)
	{
		AnimationHandler::GetInstance().Update(deltaTimeInSeconds);
	}
	else if (gameManager.GetCurrentGameState() == GameState::Paused)
	{
		gameManager.SetGameReset(false);
		InputHandler::GetInstance().Update(deltaTimeInSeconds);
		InputHandler::GetInstance().SetIsQuitButtonClicked();
		gameManager.HandleQuitButtonClick();
	}

	SoundManager::GetInstance().Cleanup();
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.)
// See App.h
//------------------------------------------------------------------------
void Render()
{
	RenderingHandler::GetInstance().Render();
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
	SpriteManager::GetInstance().DestroyAllSprites();
}