#include "stdafx.h"
#include "States/include/MainMenuState.h"
#include "Managers/include/GameManager.h"
#include "Managers/include/SoundManager.h"
#include "Managers/include/EntityManager.h"
#include "Systems/include/TitleHandler.h"
#include "Systems/include/InputHandler.h"
#include "States/include/LoadingState.h"
#include "Utilities/include/Helper.h"
using std::make_unique;

void MainMenuState::Enter()
{
}

void MainMenuState::Exit()
{
	SoundManager::GetInstance().StopSound(Helper::PATH_TO_NON_GAMEPLAY_MUSIC);
}

void MainMenuState::Update(float deltaTime) 
{
	GameManager& gameManager = GameManager::GetInstance();
	EntityId playButtonEntityId = EntityManager::GetInstance().GetPlayButtonEntityId();

	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

	if (!gameManager.GetGameReset())
	{
		gameManager.ResetGame();
		SoundManager::GetInstance().PlaySoundFromFile(Helper::PATH_TO_NON_GAMEPLAY_MUSIC, true);
		gameManager.SetGameReset(true);
	}

	TitleHandler::GetInstance().OscillateTitle(deltaTime);
	HandleButtonClick(playButtonEntityId);
}

GameState MainMenuState::GetStateEnum() const
{
	return GameState::MainMenu;
}
