#include "stdafx.h"
#include "Components/include/Timer.h"
#include "Managers/include/EntityManager.h"
#include "Managers/include/GameManager.h"
#include "States/include/LoadingState.h"
#include "Systems/include/AnimationHandler.h"
#include "ParticleSystem/include/ParticleManager.h"
#include "Systems/include/RenderingHandler.h"
#include "Systems/include/TimerHandler.h"
#include "Utilities/include/Helper.h"
#include "Utilities/include/Particle.h"
using std::make_unique;

TimerHandler& TimerHandler::GetInstance()
{
	static TimerHandler instance;
	return instance;
}

void TimerHandler::Update(float deltaTime) 
{
    GameManager& gameManager = GameManager::GetInstance();
    EntityManager& entityManager = EntityManager::GetInstance();
    AnimationHandler& animationHandler = AnimationHandler::GetInstance();
    RenderingHandler& renderingHandler = RenderingHandler::GetInstance();
    GameState currentGameState = gameManager.GetCurrentGameState()->GetStateEnum();

    for (EntityId entityId : entityManager.GetEntitiesWithComponents<Timer>())
    {
        Timer* timer = entityManager.GetComponent<Timer>(entityId);

        if (timer->GetType() == TimerType::ParticleLifespan)
        {
            CSimpleSprite* particleSprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
            Particle* particleComponent = entityManager.GetComponent<Particle>(entityId);

            float newRemainingTime = timer->GetRemainingTime() - deltaTime;
            timer->SetRemainingTime(newRemainingTime);

            if (newRemainingTime <= 0)
            {
                timer->SetRemainingTime(timer->GetInitialDuration());
                // ParticleManager::GetInstance().ReturnParticleToPool(particleComponent->GetParticleType(), entityId);
            }
        }

        else if (timer->GetType() == TimerType::Countdown)
        {
            EntityId playerEntityId = entityManager.GetPlayerEntityId();
            Tag* playerTag = entityManager.GetComponent<Tag>(playerEntityId);

            float newRemainingTime = timer->GetRemainingTime() - deltaTime;
            timer->SetRemainingTime(newRemainingTime);

            if (newRemainingTime <= 0)
            {
                timer->SetRemainingTime(0);
                playerTag->SetEntityState(EntityState::Dead);
            }
        }

        // For player, we only start decrementing remaining time if player is dead
        else if (timer->GetType() == TimerType::PlayerDeath)
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
                    gameManager.ChangeState(make_unique<LoadingState>(GameState::Gameplay));
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

        // For lightning strike, we only start decrementing remaining time if we just got it from the pool and started striking enemies
        else if (timer->GetType() == TimerType::LightningFlash) 
        {
            Tag* lightningStrikeTag = entityManager.GetComponent<Tag>(entityId);
            if (lightningStrikeTag->GetEntityState() == EntityState::Alive)
            {
                float newRemainingTime = timer->GetRemainingTime() - deltaTime;
                timer->SetRemainingTime(newRemainingTime);

                if (newRemainingTime <= 0)
                {
                    timer->SetRemainingTime(timer->GetInitialDuration());
                    entityManager.ReturnLightningStrikeToPool(entityId);
                }
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

