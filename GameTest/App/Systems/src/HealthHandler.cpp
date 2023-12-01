#include "stdafx.h"
#include "Components/include/Health.h"
#include "Components/include/Tag.h"
#include "../include/HealthHandler.h"
#include <algorithm>

void HealthHandler::ProcessEnemyHitPlayer(EntityManager& entityManager)
{
	EntityId playerEntity = entityManager.GetPlayerEntityId();
	auto tag = entityManager.GetComponent<Tag>(playerEntity);
	auto health = entityManager.GetComponent<Health>(playerEntity);

	if (!health)
		return;

	health->SetCurrentHealth(health->GetCurrentHealth() - 20);

	if (health->GetCurrentHealth() > 0)
		tag->SetEntityState(EntityState::ALIVE);

	if (health->GetCurrentHealth() <= 0) {
		// TODO: end the game
	}
}
