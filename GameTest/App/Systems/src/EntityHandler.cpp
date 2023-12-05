#include "stdafx.h"
#include "../include/EntityHandler.h"
#include "Components/include/Transform.h"
#include "Utilities/include/App.h"
#include "Utilities/include/Helper.h"
#include "Systems/include/AnimationHandler.h"
#include "Systems/include/ScreenHandler.h"
#include "Managers/include/EntityManager.h"

using glm::vec3;

void EntityHandler::HandleEvent(const Event& event, EntityManager& entityManager, float deltaTime)
{
	if (event.GetEventType() == "BulletHitEnemy") {
		HandleBulletHitEnemy(entityManager, deltaTime);
	}
	else if (event.GetEventType() == "EnemyHitPlayer") {
		HandleEnemyHitPlayer(event, entityManager, deltaTime);
	}
}

void EntityHandler::HandleBulletHitEnemy(EntityManager& entityManager, float deltaTime)
{
	constexpr int columns = 4;
	constexpr int rows = 2;
	constexpr float screenWidth = ScreenHandler::SCREEN_WIDTH;
	constexpr float screenHeight = ScreenHandler::SCREEN_HEIGHT;
	EntityId playerEntityId = entityManager.GetPlayerEntityId();
	vec3 playerPos = entityManager.GetComponent<Transform>(playerEntityId)->GetPosition();

	CSimpleSprite* rawFirstEnemySprite = App::CreateSprite(Helper::PATH_TO_ENEMY_SPRITE_SHEET, columns, rows);
	shared_ptr<CSimpleSprite> firstEnemySprite = shared_ptr<CSimpleSprite>(rawFirstEnemySprite);
	CSimpleSprite* rawSecondEnemySprite = App::CreateSprite(Helper::PATH_TO_ENEMY_SPRITE_SHEET, columns, rows);
	shared_ptr<CSimpleSprite> secondEnemySprite = shared_ptr<CSimpleSprite>(rawSecondEnemySprite);

	EntityId firstEnemyEntityId = entityManager.CreateEnemyEntity(playerPos, firstEnemySprite, screenWidth, screenHeight);
	AnimationHandler::InitEnemyAnimation(firstEnemySprite);
	EntityId secondEnemyEntityId = entityManager.CreateEnemyEntity(playerPos, secondEnemySprite, screenWidth, screenHeight);
	AnimationHandler::InitEnemyAnimation(secondEnemySprite);
}

void EntityHandler::HandleEnemyHitPlayer(Event event, EntityManager& entityManager, float deltaTime)
{
	EntityId playerEntityId, enemyEntityId;
	EntityId firstEntityId = event.GetEntities()[0];
	EntityId secondEntityId = event.GetEntities()[1];

	EntityType firstEntityType = entityManager.GetComponent<Tag>(firstEntityId)->GetEntityType();

	if (firstEntityType == EntityType::Player)
	{
		playerEntityId = firstEntityId;
		enemyEntityId = secondEntityId;
	}
	else
	{
		playerEntityId = secondEntityId;
		enemyEntityId = firstEntityId;
	}

	entityManager.MarkEntityForDeletion(enemyEntityId);
}
