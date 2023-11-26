#include "stdafx.h"
#include "../include/TimerHandler.h"

void TimerHandler::Update(EntityManager& entityManager, float deltaTime)
{
    for (auto entity : entityManager.GetEntitiesWithComponents<Timer>()) {
        auto timer = entityManager.GetComponent<Timer>(entity);
        if (timer) {
            timer->countdownTime -= deltaTime;
            if (timer->countdownTime < 0) {
                timer->countdownTime = 0;
            }
        }
    }
}
