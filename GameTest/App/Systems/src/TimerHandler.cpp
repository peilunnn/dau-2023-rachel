#include "stdafx.h"
#include "Components/include/Timer.h"
#include "Components/include/Health.h"
#include "Managers/include/EntityManager.h"
#include "Managers/include/GameManager.h"
#include "Systems/include/TimerHandler.h"
#include "Utilities/include/Helper.h"

TimerHandler& TimerHandler::GetInstance()
{
	static TimerHandler instance;
	return instance;
}

void TimerHandler::Update(float deltaTime) {
    EntityManager& entityManager = EntityManager::GetInstance();
    GameManager& gameManager = GameManager::GetInstance();

    for (EntityId entityId : entityManager.GetEntitiesWithComponents<Timer>()) {
        Timer* timer = entityManager.GetComponent<Timer>(entityId);

        // We only start decrementing remaining time if the player is dead
        if (timer->GetType() == TimerType::PlayerDeath) {
            Health* health = entityManager.GetComponent<Health>(entityId);
            int currentHealth = health->GetCurrentHealth();
            if (health->GetCurrentHealth() <= 0) 
            {
                float remainingTime = timer->GetDuration();
                timer->SetDuration(remainingTime - deltaTime);

                if (remainingTime <= 0)
                    gameManager.TransitionToLoadingState();
            }
        }

        // For all other normal timers, we decrement remaining time every frame
        else
        {
            float remainingTime = timer->GetDuration();
            timer->SetDuration(remainingTime - deltaTime);

            if (remainingTime <= 0)
                gameManager.TransitionToLoadingState();
        }
    }
}

