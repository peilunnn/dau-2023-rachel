#include "stdafx.h"
#include "../include/EntityManager.h"

Entity EntityManager::nextEntityId = -1;

Entity EntityManager::CreatePlayerEntity(CSimpleSprite* playerSprite)
{
	Entity playerEntityId = CreateEntity();

	float maxX = 800.0f;
	float maxY = 400.0f;
	float playerPosX = RandomUtility::GenerateFloat(0.0, maxX);
	float playerPosY = RandomUtility::GenerateFloat(0.0, maxY);
	float playerPosZ = 0.0f;

	auto playerTransform = std::make_shared<Transform>(glm::vec3(playerPosX, playerPosY, playerPosZ), glm::vec3(0.0f), glm::vec3(1.0f));
	auto playerRenderable = std::make_shared<Renderable>(playerSprite);
	auto playerCollider = std::make_shared<Collider>();
	playerCollider->collisionShape = CollisionShape::SPHERE;
	playerCollider->collisionMask = 1;
	auto playerVelocity = std::make_shared<Velocity>(0.0f, 0.0f);
	auto playerDirection = std::make_shared<Direction>();
	playerDirection->direction = glm::vec2(0.0f, -1.0f);
	auto playerHealth = std::make_shared<Health>();
	auto playerAnimation = std::make_shared<Animation>();

	EntityManager::AddComponent(playerEntityId, playerTransform);
	EntityManager::AddComponent(playerEntityId, playerRenderable);
	EntityManager::AddComponent(playerEntityId, playerCollider);
	EntityManager::AddComponent(playerEntityId, playerVelocity);
	EntityManager::AddComponent(playerEntityId, playerDirection);
	EntityManager::AddComponent(playerEntityId, playerHealth);
	EntityManager::AddComponent(playerEntityId, playerAnimation);

	return playerEntityId;
}


Entity EntityManager::CreateEnemyEntity(EntityManager& entityManager, const glm::vec3& playerPos, CSimpleSprite* enemySprite)
{
	Entity enemyEntity = entityManager.CreateEntity();

	float minVx = -5.0f, maxVx = 5.0f;
	float minVy = -5.0f, maxVy = 5.0f;
	float minDx = -1.0f, maxDx = 1.0f;
	float minDy = -1.0f, maxDy = 1.0f;
	float minDz = -1.0f, maxDz = 1.0f;
	glm::vec2 randomVelocity = RandomUtility::GenerateVec2(minVx, maxVx, minVy, maxVy);
	glm::vec3 randomDirection = RandomUtility::GenerateVec3(minDx, maxDx, minDy, maxDy, minDz, maxDz);
	float enemyScale = 0.2f;

	glm::vec3 enemyPos = GetOppositeQuadrantPosition(playerPos, 1024.0f, 768.0f);

	auto enemyTransform = std::make_shared<Transform>(enemyPos, glm::vec3(0.0f), glm::vec3(enemyScale));
	auto enemyRenderable = std::make_shared<Renderable>(enemySprite);
	auto enemyCollider = std::make_shared<Collider>();
	auto enemyVelocity = std::make_shared<Velocity>();
	auto enemyDirection = std::make_shared<Direction>();
	auto enemyHealth = std::make_shared<Health>();
	auto enemyAnimation = std::make_shared<Animation>();

	EntityManager::AddComponent(enemyEntity, enemyTransform);
	EntityManager::AddComponent(enemyEntity, enemyRenderable);
	EntityManager::AddComponent(enemyEntity, enemyCollider);
	EntityManager::AddComponent(enemyEntity, enemyVelocity);
	EntityManager::AddComponent(enemyEntity, enemyDirection);
	EntityManager::AddComponent(enemyEntity, enemyHealth);
	EntityManager::AddComponent(enemyEntity, enemyAnimation);

	return enemyEntity;
}

glm::vec3 EntityManager::GetOppositeQuadrantPosition(const glm::vec3& playerPos, float screenWidth, float screenHeight)
{
	glm::vec3 enemyPos;
	if (playerPos.x < screenWidth / 2) {
		enemyPos.x = screenWidth * 0.75f; // Spawn in the right half
	}
	else {
		enemyPos.x = screenWidth * 0.25f; // Spawn in the left half
	}
	if (playerPos.y < screenHeight / 2) {
		enemyPos.y = screenHeight * 0.75f; // Spawn in the lower half
	}
	else {
		enemyPos.y = screenHeight * 0.25f; // Spawn in the upper half
	}
	return enemyPos;
}
