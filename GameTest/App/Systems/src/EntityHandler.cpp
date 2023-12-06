#include "stdafx.h"
#include "../include/EntityHandler.h"
#include "Components/include/Transform.h"
#include "Components/include/Cooldown.h"
#include "Components/include/Screen.h"
#include "Utilities/include/App.h"
#include "Utilities/include/Helper.h"
#include "Systems/include/AnimationHandler.h"
#include "Managers/include/EntityManager.h"

using glm::vec3;

void EntityHandler::HandleEvent(const Event& event, float deltaTime)
{
	EntityManager& entityManager = EntityManager::GetInstance();

	if (event.GetEventType() == "BulletHitEnemy") {
		EntityId bulletEntityId = event.GetEntities()[0];
		EntityId enemyEntityId = event.GetEntities()[1];
		HandleBulletHitEnemy(entityManager, bulletEntityId, enemyEntityId, deltaTime);
	}
	else if (event.GetEventType() == "EnemyHitPlayer") {
		EntityId enemyEntityId = event.GetEntities()[0];
		HandleEnemyHitPlayer(entityManager, enemyEntityId, deltaTime);
	}
}

void EntityHandler::HandleBulletHitEnemy(EntityManager& entityManager, EntityId bulletEntityId, EntityId enemyEntityId, float deltaTime)
{
	entityManager.MarkEntityForDeletion(bulletEntityId);
	entityManager.MarkEntityForDeletion(enemyEntityId);
	SpawnTwoEnemies(entityManager, bulletEntityId, enemyEntityId, deltaTime);
}

void EntityHandler::SpawnTwoEnemies(EntityManager& entityManager, EntityId bulletEntityId, EntityId enemyEntityId, float deltaTime)
{
	constexpr int columns = 4;
	constexpr int rows = 2;
	Screen& screen = Screen::GetInstance();
	const float screenWidth = screen.SCREEN_WIDTH;
	const float screenHeight = screen.SCREEN_HEIGHT;
	EntityId playerEntityId = entityManager.GetPlayerEntityId();
	vec3 playerPos = entityManager.GetComponent<Transform>(playerEntityId)->GetPosition();

	CSimpleSprite* firstEnemySprite = App::CreateSprite(Helper::PATH_TO_ENEMY_SPRITE_SHEET, columns, rows);
	CSimpleSprite* secondEnemySprite = App::CreateSprite(Helper::PATH_TO_ENEMY_SPRITE_SHEET, columns, rows);

	EntityId firstEnemyEntityId = entityManager.CreateEnemyEntity(playerPos, firstEnemySprite, screenWidth, screenHeight);
	EntityId secondEnemyEntityId = entityManager.CreateEnemyEntity(playerPos, secondEnemySprite, screenWidth, screenHeight);
}


void EntityHandler::HandleEnemyHitPlayer(EntityManager& entityManager, EntityId enemyEntityId, float deltaTime)
{
	entityManager.MarkEntityForDeletion(enemyEntityId);
}
