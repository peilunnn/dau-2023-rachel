#include "stdafx.h"
#include "../include/TimerHandler.h"
#include "../App/Components/include/Timer.h"

void TimerHandler::Update(EntityManager& entityManager, float deltaTime)
{
	for (auto entity : entityManager.GetEntitiesWithComponents<Timer>()) {
		auto timer = entityManager.GetComponent<Timer>(entity);
		if (timer) {
			timer->SetCountdownTime(timer->GetCountdownTime() - deltaTime);
			if (timer->GetCountdownTime() < 0) {
				timer->SetCountdownTime(0);
			}
		}
	}
}
