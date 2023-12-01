#include "stdafx.h"
#include "../include/TimerHandler.h"
#include "../App/Components/include/Timer.h"

void TimerHandler::Update(EntityManager& entityManager, float deltaTime)
{
	for (auto entityId : entityManager.GetEntitiesWithComponents<Timer>()) {
		auto timer = entityManager.GetComponent<Timer>(entityId);
		auto countdownTime = timer->GetCountdownTime();
		
		timer->SetCountdownTime(countdownTime - deltaTime);
		
		if (countdownTime <= 0) {
			// TODO: end the game
		}
	}
}
