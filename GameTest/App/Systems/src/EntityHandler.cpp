#include "stdafx.h"
#include "Managers/include/EntityManager.h"
#include "Systems/include/EntityHandler.h"

using glm::vec3;

EntityHandler& EntityHandler::GetInstance()
{
	static EntityHandler instance;
	return instance;
}

void EntityHandler::HandleEvent(const Event &event, float deltaTime)
{
	EntityManager &entityManager = EntityManager::GetInstance();

	if (event.GetEventType() == EventType::BulletHitEnemy)
	{
		EntityId bulletEntityId = event.GetEntities()[0];
		EntityId enemyEntityId = event.GetEntities()[1];
		HandleBulletHitEnemy(entityManager, bulletEntityId, enemyEntityId, deltaTime);
	}
	else if (event.GetEventType() == EventType::EnemyHitPlayer)
	{
		EntityId enemyEntityId = event.GetEntities()[0];
		HandleEnemyHitPlayer(entityManager, enemyEntityId);
	}
	else if (event.GetEventType() == EventType::BulletOutOfBounds)
	{
		EntityId bulletEntityId = event.GetEntities()[0];
		HandleBulletOutOfBounds(entityManager, bulletEntityId);
	}
}

void EntityHandler::HandleBulletHitEnemy(EntityManager &entityManager, EntityId bulletEntityId, EntityId enemyEntityId, float deltaTime)
{
	entityManager.MarkEntityForDeletion(bulletEntityId);
	entityManager.MarkEntityForDeletion(enemyEntityId);
	SpawnTwoEnemies(entityManager, bulletEntityId, enemyEntityId, deltaTime);
}

void EntityHandler::SpawnTwoEnemies(EntityManager &entityManager, EntityId bulletEntityId, EntityId enemyEntityId, float deltaTime)
{
	SpriteManager& spriteManager = SpriteManager::GetInstance();
	EntityId firstEnemyEntityId = entityManager.CreateEnemyEntity(spriteManager);
	EntityId secondEnemyEntityId = entityManager.CreateEnemyEntity(spriteManager);
}

void EntityHandler::HandleEnemyHitPlayer(EntityManager &entityManager, EntityId enemyEntityId)
{
	entityManager.MarkEntityForDeletion(enemyEntityId);
}

void EntityHandler::HandleBulletOutOfBounds(EntityManager &entityManager, EntityId bulletEntityId)
{
	entityManager.MarkEntityForDeletion(bulletEntityId);
}