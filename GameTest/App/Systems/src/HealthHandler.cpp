#include "stdafx.h"
#include "Components/include/Health.h"
#include "Components/include/Tag.h"
#include "Systems/include/HealthHandler.h"
#include "Utilities/include/Helper.h"

void HealthHandler::HandleEvent(const Event &event, float deltaTime)
{
	EntityManager &entityManager = EntityManager::GetInstance();

	if (event.GetEventType() == "EnemyHitPlayer")
	{
		HandleEnemyHitPlayer(entityManager);
	}
}

void HealthHandler::HandleEnemyHitPlayer(EntityManager &entityManager)
{
	EntityId playerEntityId = entityManager.GetPlayerEntityId();
	Tag *tag = entityManager.GetComponent<Tag>(playerEntityId);
	Health *health = entityManager.GetComponent<Health>(playerEntityId);

	if (!health)
		return;

	int currentHealth = health->GetCurrentHealth();
	health->SetCurrentHealth(currentHealth - m_healthReduction);

	if (currentHealth > 0)
		tag->SetEntityState(EntityState::Alive);
	else
	{
		// TODO: end the game
		Helper::Log("game over");
	}
}
