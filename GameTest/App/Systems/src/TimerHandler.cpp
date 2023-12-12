#include "stdafx.h"
#include "Components/include/Health.h"
#include "Components/include/Timer.h"
#include "Managers/include/EntityManager.h"
#include "Managers/include/GameManager.h"
#include "Systems/include/AnimationHandler.h"
#include "Systems/include/RenderingHandler.h"
#include "Systems/include/TimerHandler.h"
#include "Utilities/include/Helper.h"

TimerHandler& TimerHandler::GetInstance()
{
	static TimerHandler instance;
	return instance;
}

void TimerHandler::Update(float deltaTime) {
    GameManager& gameManager = GameManager::GetInstance();
    EntityManager& entityManager = EntityManager::GetInstance();
    AnimationHandler& animationHandler = AnimationHandler::GetInstance();
    RenderingHandler& renderingHandler = RenderingHandler::GetInstance();

    if (gameManager.GetCurrentGameState() == GameState::Paused)
        return;

    for (EntityId entityId : entityManager.GetEntitiesWithComponents<Timer>())
    {
        Timer* timer = entityManager.GetComponent<Timer>(entityId);

        // We only start decrementing remaining time if the player is dead
        if (timer->GetType() == TimerType::PlayerDeath) {
            Health* health = entityManager.GetComponent<Health>(entityId);
            int currentHealth = health->GetCurrentHealth();
            if (health->GetCurrentHealth() <= 0) 
            {
                float newRemainingTime = timer->GetRemainingTime() - deltaTime;
                timer->SetRemainingTime(newRemainingTime);

                if (newRemainingTime > 0)
                {
                    animationHandler.RotatePlayer(deltaTime);
                    renderingHandler.UpdateFade(deltaTime);
                }
                else
                    gameManager.TransitionToLoadingState();
            }
        }

        // For all other normal timers, we decrement remaining time every frame
        else
        {
            float remainingTime = timer->GetRemainingTime();
            timer->SetRemainingTime(remainingTime - deltaTime);

            if (remainingTime <= 0)
            {
                Helper::Log("countdown reached 0");
                gameManager.TransitionToLoadingState();
            }
        }
    }
}

void TimerHandler::ResetTimers()
{
    EntityManager& entityManager = EntityManager::GetInstance();

    for (EntityId entityId : entityManager.GetEntitiesWithComponents<Timer>())
    {
        Timer* timer = entityManager.GetComponent<Timer>(entityId);
        timer->SetRemainingTime(timer->GetInitialDuration());
    }
}

