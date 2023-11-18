#include "stdafx.h"
#include "../include/EntityManager.h"

Entity EntityManager::nextEntityId = 0;

Entity EntityManager::CreatePlayerEntity(CSimpleSprite* playerSprite)
{
	Entity playerEntityId = CreateEntity();

	float maxX = 800.0f;
	float maxY = 400.0f;
	float playerPosX = Helper::GenerateFloat(0.0, maxX);
	float playerPosY = Helper::GenerateFloat(0.0, maxY);
	float playerPosZ = 0.0f;

	auto playerTag = std::make_shared<Tag>(EntityType::PLAYER);
	auto playerTransform = std::make_shared<Transform>(glm::vec3(playerPosX, playerPosY, playerPosZ), glm::vec3(0.0f), glm::vec3(1.0f));
	auto playerRenderable = std::make_shared<Renderable>(playerSprite);
	auto playerCollider = std::make_shared<Collider>();
	playerCollider->collisionShape = CollisionShape::SPHERE;
	playerCollider->collisionMask = 1;
	auto playerVelocity = std::make_shared<Velocity>(0.0f, 0.0f);
	auto playerHealth = std::make_shared<Health>();
	auto playerAnimation = std::make_shared<Animation>();

	EntityManager::AddComponent(playerEntityId, playerTag);
	EntityManager::AddComponent(playerEntityId, playerTransform);
	EntityManager::AddComponent(playerEntityId, playerRenderable);
	EntityManager::AddComponent(playerEntityId, playerCollider);
	EntityManager::AddComponent(playerEntityId, playerVelocity);
	EntityManager::AddComponent(playerEntityId, playerHealth);
	EntityManager::AddComponent(playerEntityId, playerAnimation);

	return playerEntityId;
}


Entity EntityManager::CreateEnemyEntity(EntityManager& entityManager, const glm::vec3& playerPos, CSimpleSprite* enemySprite, float screenWidth, float screenHeight)
{
	Entity enemyEntity = entityManager.CreateEntity();

	float minVx = -0.1f, maxVx = 0.1f;
	float minVy = -0.1f, maxVy = 0.1f;
	glm::vec2 randomVelocity = Helper::GenerateVec2(minVx, maxVx, minVy, maxVy);
	float enemyScale = 0.5f;

	glm::vec3 enemyPos = GetOppositeQuadrantPosition(playerPos, 1024.0f, 768.0f);

	auto enemyTag = std::make_shared<Tag>(EntityType::ENEMY);
	auto enemyTransform = std::make_shared<Transform>(enemyPos, glm::vec3(0.0f), glm::vec3(enemyScale));
	auto enemyRenderable = std::make_shared<Renderable>(enemySprite);
	auto enemyCollider = std::make_shared<Collider>();
	auto enemyVelocity = std::make_shared<Velocity>(randomVelocity.x, randomVelocity.y);
	auto enemyDirection = std::make_shared<Direction>();
	auto enemyHealth = std::make_shared<Health>();
	auto enemyAnimation = std::make_shared<Animation>();

	EntityManager::AddComponent(enemyEntity, enemyTag);
	EntityManager::AddComponent(enemyEntity, enemyTransform);
	EntityManager::AddComponent(enemyEntity, enemyRenderable);
	EntityManager::AddComponent(enemyEntity, enemyCollider);
	EntityManager::AddComponent(enemyEntity, enemyVelocity);
	EntityManager::AddComponent(enemyEntity, enemyDirection);
	EntityManager::AddComponent(enemyEntity, enemyHealth);
	EntityManager::AddComponent(enemyEntity, enemyAnimation);

	return enemyEntity;
}

Entity EntityManager::CreateBulletEntity(CSimpleSprite* bulletSprite, const glm::vec3& position, const glm::vec2& velocity)
{
	Entity bulletEntity = CreateEntity();

	float bulletScale = 2.0f;

	auto bulletTag = std::make_shared<Tag>(EntityType::BULLET);
	auto bulletTransform = std::make_shared<Transform>(position, glm::vec3(0.0f), glm::vec3(bulletScale));
	auto bulletRenderable = std::make_shared<Renderable>(bulletSprite);
	auto bulletCollider = std::make_shared<Collider>();
	auto bulletVelocity = std::make_shared<Velocity>(velocity.x, velocity.y);
	auto bulletDamage = std::make_shared<Damage>();

	AddComponent(bulletEntity, bulletTag);
	AddComponent(bulletEntity, bulletTransform);
	AddComponent(bulletEntity, bulletRenderable);
	AddComponent(bulletEntity, bulletCollider);
	AddComponent(bulletEntity, bulletVelocity);
	AddComponent(bulletEntity, bulletDamage);

	return bulletEntity;
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
