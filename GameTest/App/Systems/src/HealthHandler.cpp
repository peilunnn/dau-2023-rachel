#include "stdafx.h"
#include "Components/include/Health.h"
#include "Components/include/Tag.h"
#include "../include/HealthHandler.h"
#include "Utilities/include/Helper.h"
#include <algorithm>

void HealthHandler::ProcessEnemyHitPlayer(EntityManager &entityManager)
{
	EntityId playerEntityId = entityManager.GetPlayerEntityId();
	auto tag = entityManager.GetComponent<Tag>(playerEntityId);
	auto health = entityManager.GetComponent<Health>(playerEntityId);

	if (!health)
		return;

	auto currentHealth = health->GetCurrentHealth();
	health->SetCurrentHealth(currentHealth - 20);

	if (currentHealth > 0)
		tag->SetEntityState(EntityState::Alive);
	else
	{
		// TODO: end the game
		Helper::Log("game over");
	}
}
