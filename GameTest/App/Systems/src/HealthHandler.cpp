#include "stdafx.h"
#include "../include/HealthHandler.h"

void HealthHandler::ProcessEnemyHitPlayer(EntityManager& entityManager)
{
	EntityId playerEntity = entityManager.GetPlayerEntityId();
	auto tag = entityManager.GetComponent<Tag>(playerEntity);
	auto health = entityManager.GetComponent<Health>(playerEntity);
	
	if (!health)
		return;

	health->currentHealth -= 20;

	if (health->currentHealth > 0)
		tag->entityState = EntityState::ALIVE;

	if (health->currentHealth <= 0) {
		// TODO: end the game
	}
}
