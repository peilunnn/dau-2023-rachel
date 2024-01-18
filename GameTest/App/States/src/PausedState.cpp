#include "stdafx.h"
#include "States/include/PausedState.h"
#include "Managers/include/GameManager.h"
#include "Managers/include/SoundManager.h"
#include "Managers/include/EntityManager.h"
#include "Systems/include/InputHandler.h"
#include "Utilities/include/Helper.h"
using std::make_unique;

void PausedState::Enter()
{
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	GameManager::GetInstance().SetGameReset(false);
	SoundManager::GetInstance().PauseSound(Helper::PATH_TO_GAMEPLAY_MUSIC);
}

void PausedState::Exit()
{
}

void PausedState::Update(float deltaTime)
{
	EntityId quitButtonEntityId = EntityManager::GetInstance().GetQuitButtonEntityId();
	HandleButtonClick(quitButtonEntityId);
	InputHandler::GetInstance().HandlePauseInput();
}

GameState PausedState::GetStateEnum() const
{
	return GameState::Paused;
}