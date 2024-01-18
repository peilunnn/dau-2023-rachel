#include "stdafx.h"
#include "States/include/LoadingState.h"
#include "States/include/GameOverState.h"
#include "States/include/MainMenuState.h"
#include "Systems/include/AnimationHandler.h"
#include "Managers/include/GameManager.h"
#include "States/include/GameplayState.h"
using std::make_unique;

LoadingState::LoadingState(GameState previousState)
	: m_previousState(previousState), m_timeSpentInLoading(0.0f) {}

void LoadingState::Enter()
{
	m_timeSpentInLoading = 0.0f;
}

void LoadingState::Exit()
{
}

void LoadingState::Update(float deltaTime)
{
	m_timeSpentInLoading += deltaTime;
	GameManager& gameManager = GameManager::GetInstance();

	glutSetCursor(GLUT_CURSOR_NONE);
	AnimationHandler::GetInstance().Update(deltaTime);

	if (m_timeSpentInLoading >= m_loadingDuration)
	{
		if (m_previousState == GameState::MainMenu)
			gameManager.ChangeState(make_unique<GameplayState>());
		else if (m_previousState == GameState::Gameplay)
			gameManager.ChangeState(make_unique<GameOverState>());
		else if (m_previousState == GameState::GameOver || m_previousState == GameState::Paused)
			gameManager.ChangeState(make_unique<MainMenuState>());
	}
}

void LoadingState::HandleButtonClick()
{
}

GameState LoadingState::GetStateEnum() const
{
	return GameState::Loading;
}
