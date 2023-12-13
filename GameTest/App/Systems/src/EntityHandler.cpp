#include "stdafx.h"
#include "Components/include/Renderable.h"
#include "Components/include/Screen.h"
#include "Components/include/Transform.h"
#include "Components/include/Velocity.h"
#include "Managers/include/EntityManager.h"
#include "Systems/include/EntityHandler.h"
#include "Utilities/include/Helper.h"

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
		HandleBulletHitEnemy(entityManager, bulletEntityId, enemyEntityId);
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

void EntityHandler::InitializeEnemy(EntityManager& entityManager, EntityId enemyEntityId)
{
	SpriteManager& spriteManager = SpriteManager::GetInstance();
	Screen& screen = Screen::GetInstance();

	EntityId playerEntityId = entityManager.GetPlayerEntityId();
	vec3 playerPos = entityManager.GetComponent<Transform>(playerEntityId)->GetPosition();
	const float borderWidth = (screen.BORDER_RIGHT_SCREEN_COORD - screen.BORDER_LEFT_SCREEN_COORD);
	const float borderHeight = (screen.BORDER_BOTTOM_SCREEN_COORD - screen.BORDER_TOP_SCREEN_COORD);
	vec3 enemyPos = Helper::GetOppositeQuadrantPosition(playerPos, borderWidth, borderHeight);

	Transform* enemyTransform = entityManager.GetComponent<Transform>(enemyEntityId);
	enemyTransform->SetPosition(enemyPos);

	CSimpleSprite* enemySprite = entityManager.GetComponent<Renderable>(enemyEntityId)->GetSprite();
	enemySprite->SetIsVisible(true);

	constexpr float minVx = -100.0f;
	constexpr float maxVx = 300.0f;
	constexpr float minVy = -100.0f;
	constexpr float maxVy = 300.0f;
	vec2 randomVelocity = Helper::GenerateVec2(minVx, maxVx, minVy, maxVy);
	Velocity* enemyVelocity = entityManager.GetComponent<Velocity>(enemyEntityId);
	enemyVelocity->SetVelocity(randomVelocity);
}

void EntityHandler::HandleBulletHitEnemy(EntityManager &entityManager, EntityId bulletEntityId, EntityId enemyEntityId)
{
	SpawnTwoEnemies();
	entityManager.ReturnBulletToPool(bulletEntityId);
	entityManager.ReturnEnemyToPool(enemyEntityId);
}

void EntityHandler::SpawnTwoEnemies()
{
	EntityManager &entityManager = EntityManager::GetInstance();
	SpriteManager& spriteManager = SpriteManager::GetInstance();

	EntityId firstEnemyEntityId = entityManager.GetEnemyFromPool();
	EntityId secondEnemyEntityId = entityManager.GetEnemyFromPool();
	InitializeEnemy(entityManager, firstEnemyEntityId);
	InitializeEnemy(entityManager, secondEnemyEntityId);
}

void EntityHandler::HandleEnemyHitPlayer(EntityManager &entityManager, EntityId enemyEntityId)
{
	entityManager.ReturnEnemyToPool(enemyEntityId);
}

void EntityHandler::HandleBulletOutOfBounds(EntityManager &entityManager, EntityId bulletEntityId)
{
	entityManager.ReturnBulletToPool(bulletEntityId);
}