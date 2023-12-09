#include "stdafx.h"
#include "Components/include/Timer.h"
#include "Managers/include/EntityManager.h"
#include "Managers/include/GameManager.h"
#include "Systems/include/TimerHandler.h"

void TimerHandler::Update(float deltaTime)
{
	EntityManager& entityManager = EntityManager::GetInstance();
	GameManager& gameManager = GameManager::GetInstance();

	for (EntityId entityId : entityManager.GetEntitiesWithComponents<Timer>()) {
		Timer* timer = entityManager.GetComponent<Timer>(entityId);
		float countdownTime = timer->GetDuration();
		
		timer->SetDuration(countdownTime - deltaTime);
		
		if (countdownTime <= 0)
		{
			gameManager.SetPreviousGameState(GameState::Gameplay);
			gameManager.SetCurrentGameState(GameState::Loading);
		}
	}
}
