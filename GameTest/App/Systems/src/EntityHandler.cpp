#include "stdafx.h"
#include "Components/include/Screen.h"
#include "Components/include/Transform.h"
#include "Managers/include/EntityManager.h"
#include "Systems/include/EntityHandler.h"
#include "Utilities/include/App.h"
#include "Utilities/include/Helper.h"

using glm::vec3;

void EntityHandler::HandleEvent(const Event &event, float deltaTime)
{
	EntityManager &entityManager = EntityManager::GetInstance();

	if (event.GetEventType() == "BulletHitEnemy")
	{
		EntityId bulletEntityId = event.GetEntities()[0];
		EntityId enemyEntityId = event.GetEntities()[1];
		HandleBulletHitEnemy(entityManager, bulletEntityId, enemyEntityId, deltaTime);
	}
	else if (event.GetEventType() == "EnemyHitPlayer")
	{
		EntityId enemyEntityId = event.GetEntities()[0];
		HandleEnemyHitPlayer(entityManager, enemyEntityId);
	}
	else if (event.GetEventType() == "BulletOutOfBounds")
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
	Screen &screen = Screen::GetInstance();
	SpriteManager& spriteManager = SpriteManager::GetInstance();

	constexpr int columns = 4;
	constexpr int rows = 2;
	const float screenWidth = screen.SCREEN_WIDTH;
	const float screenHeight = screen.SCREEN_HEIGHT;
	EntityId playerEntityId = entityManager.GetPlayerEntityId();
	vec3 playerPos = entityManager.GetComponent<Transform>(playerEntityId)->GetPosition();

	EntityId firstEnemyEntityId = entityManager.CreateEnemyEntity(spriteManager, playerPos, screenWidth, screenHeight);
	EntityId secondEnemyEntityId = entityManager.CreateEnemyEntity(spriteManager, playerPos, screenWidth, screenHeight);
}

void EntityHandler::HandleEnemyHitPlayer(EntityManager &entityManager, EntityId enemyEntityId)
{
	entityManager.MarkEntityForDeletion(enemyEntityId);
}

void EntityHandler::HandleBulletOutOfBounds(EntityManager &entityManager, EntityId bulletEntityId)
{
	entityManager.MarkEntityForDeletion(bulletEntityId);
}