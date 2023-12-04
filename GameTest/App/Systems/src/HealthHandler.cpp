#include "stdafx.h"
#include "Components/include/Health.h"
#include "Components/include/Tag.h"
#include "Utilities/include/Helper.h"
#include "../include/HealthHandler.h"
#include <algorithm>

void HealthHandler::HandleEnemyHitPlayer(EntityManager &entityManager)
{
	EntityId playerEntityId = entityManager.GetPlayerEntityId();
	shared_ptr<Tag> tag = entityManager.GetComponent<Tag>(playerEntityId);
	shared_ptr<Health> health = entityManager.GetComponent<Health>(playerEntityId);

	if (!health)
		return;

	int currentHealth = health->GetCurrentHealth();
	health->SetCurrentHealth(currentHealth - healthReduction);

	if (currentHealth > 0)
		tag->SetEntityState(EntityState::Alive);
	else
	{
		// TODO: end the game
		Helper::Log("game over");
	}
}
