#include "stdafx.h"
#include "Components/include/Timer.h"
#include "Managers/include/EntityManager.h"
#include "Systems/include/TimerHandler.h"

void TimerHandler::Update(float deltaTime)
{
	EntityManager& entityManager = EntityManager::GetInstance();

	for (EntityId entityId : entityManager.GetEntitiesWithComponents<Timer>()) {
		Timer* timer = entityManager.GetComponent<Timer>(entityId);
		float countdownTime = timer->GetCountdownTime();
		
		timer->SetCountdownTime(countdownTime - deltaTime);
		
		if (countdownTime <= 0) {
			// TODO: end the game
		}
	}
}
