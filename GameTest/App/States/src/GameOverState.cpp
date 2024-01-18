#include "stdafx.h"
#include "States/include/GameOverState.h"
#include "Managers/include/GameManager.h"
#include "Managers/include/SoundManager.h"
#include "Managers/include/EntityManager.h"
#include "Systems/include/TitleHandler.h"
#include "Systems/include/InputHandler.h"
#include "States/include/LoadingState.h"
#include "Utilities/include/Helper.h"
using std::make_unique;

void GameOverState::Enter()
{
}

void GameOverState::Exit()
{
}

void GameOverState::Update(float deltaTime)
{
	GameManager& gameManager = GameManager::GetInstance();

	glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

	if (!gameManager.GetGameReset())
	{
		gameManager.ResetGame();
		SoundManager::GetInstance().PlaySoundFromFile(Helper::PATH_TO_NON_GAMEPLAY_MUSIC, true);
		gameManager.SetGameReset(true);
	}

	TitleHandler::GetInstance().OscillateTitle(deltaTime);
	HandleButtonClick();
}

void GameOverState::HandleButtonClick()
{
	GameManager& gameManager = GameManager::GetInstance();
	EntityId backButtonEntityId = EntityManager::GetInstance().GetBackButtonEntityId();
	InputHandler& inputHandler = InputHandler::GetInstance();

	if (!inputHandler.IsButtonClicked(backButtonEntityId))
		return;

	SoundManager::GetInstance().PlaySoundFromFile(Helper::PATH_TO_BUTTON_CLICK);
	gameManager.ChangeState(std::make_unique<LoadingState>(GameState::Gameplay));
}

GameState GameOverState::GetStateEnum() const
{
	return GameState::GameOver;
}
