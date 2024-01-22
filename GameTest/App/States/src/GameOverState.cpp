#include "stdafx.h"
#include "Managers/include/EntityManager.h"
#include "Managers/include/GameManager.h"
#include "Managers/include/SoundManager.h"
#include "States/include/GameOverState.h"
#include "States/include/GameOverState.h"
#include "Utilities/include/Helper.h"
using std::make_unique;

void GameOverState::Enter()
{
	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

	SoundManager::GetInstance().StopSound(Helper::PATH_TO_GAMEPLAY_MUSIC);
	SoundManager::GetInstance().PlaySoundFromFile(Helper::PATH_TO_NON_GAMEPLAY_MUSIC, true);
}

void GameOverState::Exit()
{
	GameManager::GetInstance().SetGameReset(false);
}

void GameOverState::Update(float deltaTime)
{
	GameManager& gameManager = GameManager::GetInstance();
	EntityId backButtonEntityId = EntityManager::GetInstance().GetBackButtonEntityId();

	HandleButtonClick(backButtonEntityId);
}

GameState GameOverState::GetStateEnum() const
{
	return GameState::GameOver;
}
