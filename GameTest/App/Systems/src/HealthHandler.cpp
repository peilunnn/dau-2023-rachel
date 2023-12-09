#include "stdafx.h"
#include "Components/include/Health.h"
#include "Components/include/Tag.h"
#include "Managers/include/GameManager.h"
#include "Systems/include/HealthHandler.h"
#include "Utilities/include/Helper.h"

HealthHandler& HealthHandler::GetInstance()
{
	static HealthHandler instance;
	return instance;
}

void HealthHandler::HandleEvent(const Event &event, float deltaTime)
{
	EntityManager &entityManager = EntityManager::GetInstance();

	if (event.GetEventType() == "EnemyHitPlayer")
		HandleEnemyHitPlayer(entityManager);
}

void HealthHandler::HandleEnemyHitPlayer(EntityManager &entityManager)
{
	GameManager& gameManager = GameManager::GetInstance();
	EntityId playerEntityId = entityManager.GetPlayerEntityId();
	Tag *tag = entityManager.GetComponent<Tag>(playerEntityId);
	Health *health = entityManager.GetComponent<Health>(playerEntityId);

	if (!health)
		return;

	int newHealth = health->GetCurrentHealth() - m_healthReduction;
	health->SetCurrentHealth(newHealth);

	if (newHealth > 0)
		tag->SetEntityState(EntityState::Alive);
	else
		gameManager.TransitionToLoadingState();
}
