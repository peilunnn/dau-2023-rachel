#include "stdafx.h"
#include "../include/TimerHandler.h"
#include "../App/Components/include/Timer.h"

void TimerHandler::Update(EntityManager& entityManager, float deltaTime)
{
	for (EntityId entityId : entityManager.GetEntitiesWithComponents<Timer>()) {
		shared_ptr<Timer> timer = entityManager.GetComponent<Timer>(entityId);
		float countdownTime = timer->GetCountdownTime();
		
		timer->SetCountdownTime(countdownTime - deltaTime);
		
		if (countdownTime <= 0) {
			// TODO: end the game
		}
	}
}
