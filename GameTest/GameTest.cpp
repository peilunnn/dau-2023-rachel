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
#include "Systems/include/EntityHandler.h"
#include "Systems/include/RenderingHandler.h"
#include "Systems/include/TimerHandler.h"
#include "Systems/include/TitleHandler.h"
#include "Utilities/include/Helper.h"
#include "Utilities/include/main.h"

//------------------------------------------------------------------------
CProfiler gInputHandlerProfiler;
CProfiler gMovementHandlerProfiler;
CProfiler gCollisionHandlerProfiler;
CProfiler gRenderingHandlerProfiler;
CProfiler gAnimationHandlerProfiler;
CProfiler gTimerHandlerProfiler;
CProfiler gCooldownHandlerProfiler;

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{	
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
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

		if (!gameManager.GetGameReset())
		{
			gameManager.ResetGame();
			SoundManager::GetInstance().PlaySoundFromFile(Helper::PATH_TO_NON_GAMEPLAY_MUSIC, true);
			gameManager.SetGameReset(true);
		}
		TitleHandler::GetInstance().OscillateTitle(deltaTimeInSeconds);
		gameManager.HandlePlayButtonClick();
	}
	else if (gameManager.GetCurrentGameState() == GameState::Gameplay)
	{
		glutSetCursor(GLUT_CURSOR_NONE);

		gInputHandlerProfiler.Start();
		InputHandler::GetInstance().Update(deltaTimeInSeconds);
		gInputHandlerProfiler.Stop();

		gMovementHandlerProfiler.Start();
		MovementHandler::GetInstance().Update(deltaTimeInSeconds);
		gMovementHandlerProfiler.Stop();

		gCollisionHandlerProfiler.Start();
		CollisionHandler::GetInstance().Update(deltaTimeInSeconds);
		gCollisionHandlerProfiler.Stop();

		gRenderingHandlerProfiler.Start();
		RenderingHandler::GetInstance().UpdateScreenShakeTimer(deltaTime);
		gRenderingHandlerProfiler.Stop();

		gAnimationHandlerProfiler.Start();
		AnimationHandler::GetInstance().Update(deltaTimeInSeconds);
		gAnimationHandlerProfiler.Stop();

		gTimerHandlerProfiler.Start();
		TimerHandler::GetInstance().Update(deltaTimeInSeconds);
		gTimerHandlerProfiler.Stop();

		gCooldownHandlerProfiler.Start();
		CooldownHandler::GetInstance().Update(deltaTimeInSeconds);
		gCooldownHandlerProfiler.Stop();

		SystemManager::GetInstance().ProcessEvents(deltaTimeInSeconds);

		if (!GameManager::GetInstance().GetIsFirstEnemyInit())
			EntityHandler::GetInstance().InitEnemy();
	}
	else if (gameManager.GetCurrentGameState() == GameState::GameOver)
	{
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		gameManager.SetGameReset(false);
		gameManager.HandleBackButtonClick();
	}
	else if (gameManager.GetCurrentGameState() == GameState::Loading)
	{
		glutSetCursor(GLUT_CURSOR_NONE);
		AnimationHandler::GetInstance().Update(deltaTimeInSeconds);
	}
	else if (gameManager.GetCurrentGameState() == GameState::Paused)
	{
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
		gameManager.SetGameReset(false);
		InputHandler::GetInstance().Update(deltaTimeInSeconds);
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
	gInputHandlerProfiler.Print(300, 100, "InputHandler");
	gMovementHandlerProfiler.Print(300, 85, "MovementHandler");
	gCollisionHandlerProfiler.Print(300, 70, "CollisionHandler");
	gRenderingHandlerProfiler.Print(300, 55, "RenderingHandler");;
	gAnimationHandlerProfiler.Print(300, 40, "AnimationHandler");;
	gTimerHandlerProfiler.Print(300, 25, "TimerHandler");;
	gCooldownHandlerProfiler.Print(300, 10, "CooldownHandler");;
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
	SpriteManager::GetInstance().DestroyAllSprites();
}