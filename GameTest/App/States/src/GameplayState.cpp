#include "stdafx.h"
#include "Managers/include/GameManager.h"
#include "Managers/include/SoundManager.h"
#include "ParticleSystem/include/ParticleManager.h"
#include "States/include/GameplayState.h"
#include "Systems/include/AnimationHandler.h"
#include "Systems/include/CollisionHandler.h"
#include "Systems/include/CooldownHandler.h"
#include "Systems/include/EntityHandler.h"
#include "Systems/include/InputHandler.h"
#include "Systems/include/MovementHandler.h"
#include "Systems/include/RenderingHandler.h"
#include "Systems/include/TimerHandler.h"
#include "Utilities/include/Helper.h"
#include "Utilities/include/main.h"
using std::make_unique;

CProfiler gInputHandlerProfiler;
CProfiler gMovementHandlerProfiler;
CProfiler gCollisionHandlerProfiler;
CProfiler gRenderingHandlerProfiler;
CProfiler gAnimationHandlerProfiler;
CProfiler gTimerHandlerProfiler;
CProfiler gCooldownHandlerProfiler;

void GameplayState::Enter()
{
	GameManager& gameManager = GameManager::GetInstance();
	GameState previousGameState = gameManager.GetPreviousGameState()->GetStateEnum();
	
	if (previousGameState == GameState::Loading)
		SoundManager::GetInstance().PlaySoundFromFile(Helper::PATH_TO_GAMEPLAY_MUSIC, true);
	else if (previousGameState == GameState::Paused)
		SoundManager::GetInstance().ResumeSound(Helper::PATH_TO_GAMEPLAY_MUSIC);
}

void GameplayState::Exit()
{
}

void GameplayState::Update(float deltaTime)
{
	GameManager& gameManager = GameManager::GetInstance();

	gameManager.UpdateCrosshairPosition();
	InputHandler::GetInstance().HandlePauseInput();

	InputHandler::GetInstance().Update(deltaTime);
	MovementHandler::GetInstance().Update(deltaTime);
	ParticleManager::GetInstance().Update(deltaTime);
	CollisionHandler::GetInstance().Update(deltaTime);
	RenderingHandler::GetInstance().UpdateScreenShakeTimer(deltaTime);
	AnimationHandler::GetInstance().Update(deltaTime);
	TimerHandler::GetInstance().Update(deltaTime);
	CooldownHandler::GetInstance().Update(deltaTime);

	SystemManager::GetInstance().ProcessEvents(deltaTime);

	if (!gameManager.GetIsFirstEnemyInit())
		EntityHandler::GetInstance().InitEnemy();
}

GameState GameplayState::GetStateEnum() const
{
	return GameState::Gameplay;
}
