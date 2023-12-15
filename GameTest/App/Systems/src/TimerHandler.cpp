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

    if (gameManager.GetCurrentGameState() == GameState::MainMenu || gameManager.GetCurrentGameState() == GameState::Paused)
        return;

    for (EntityId entityId : entityManager.GetEntitiesWithComponents<Timer>())
    {
        Timer* timer = entityManager.GetComponent<Timer>(entityId);

        // For player, we only start decrementing remaining time if player is dead
        if (timer->GetType() == TimerType::PlayerDeath)
        {
            Tag* playerTag = entityManager.GetComponent<Tag>(entityId);
            if (playerTag->GetEntityState() == EntityState::Dead)
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

        // For enemy, we only start decrementing remaining time if enemy is hit by bullet
        else if (timer->GetType() == TimerType::EnemyMelt)
        {
            Tag* enemyTag = entityManager.GetComponent<Tag>(entityId);
            if (enemyTag->GetEntityState() == EntityState::HitByBullet)
            {
                float newRemainingTime = timer->GetRemainingTime() - deltaTime;
                timer->SetRemainingTime(newRemainingTime);

                if (newRemainingTime > 0)
                {
                    animationHandler.PlayMeltAnimation(entityId, deltaTime);
                }
                else
                {
                    timer->SetRemainingTime(timer->GetInitialDuration());
                    entityManager.ReturnEnemyToPool(entityId);
                }

            }
        }

        // For all other normal timers, we decrement remaining time every frame
        else
        {
            float remainingTime = timer->GetRemainingTime();
            timer->SetRemainingTime(remainingTime - deltaTime);

            if (remainingTime <= 0)
                gameManager.TransitionToLoadingState();
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

