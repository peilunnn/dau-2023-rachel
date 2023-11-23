#include "stdafx.h"
#include "../include/EntityManager.h"

Entity EntityManager::nextEntityId = 0;

std::vector<Entity> EntityManager::GetAllEntities()
{
	std::vector<Entity> allEntities;
	for (const auto& pair : entityComponents) {
		allEntities.push_back(pair.first);
	}
	return allEntities;
}

Entity EntityManager::CreatePlayerEntity(std::shared_ptr<CSimpleSprite> playerSprite)
{
	Entity playerEntity = CreateEntity();
	float maxX = 800.0f;
	float maxY = 400.0f;
	float playerPosX = Helper::GenerateFloat(0.0, maxX);
	float playerPosY = Helper::GenerateFloat(0.0, maxY);
	float playerPosZ = 0.0f;
	SpriteDimensions playerDimensions = Helper::GetSpriteDimensions(playerSprite, 1.0f);

	auto playerTag = std::make_shared<Tag>(EntityType::PLAYER);
	auto playerTransform = std::make_shared<Transform>(glm::vec3(playerPosX, playerPosY, playerPosZ), glm::vec3(0.0f), glm::vec3(1.0f));
	auto playerRenderable = std::make_shared<Renderable>(playerSprite);
	auto playerCollider = std::make_shared<Collider>();
	playerCollider->collisionShape = CollisionShape::CAPSULE;
	playerCollider->collisionType = CollisionType::PLAYER;
	playerCollider->collisionMask = static_cast<int>(CollisionType::ENEMY);
	playerCollider->radius = playerDimensions.height / 2;
	auto playerVelocity = std::make_shared<Velocity>(0.0f, 0.0f);
	auto playerHealth = std::make_shared<Health>();
	auto playerAnimation = std::make_shared<Animation>();

	EntityManager::AddComponent(playerEntity, playerTag);
	EntityManager::AddComponent(playerEntity, playerTransform);
	EntityManager::AddComponent(playerEntity, playerRenderable);
	EntityManager::AddComponent(playerEntity, playerCollider);
	EntityManager::AddComponent(playerEntity, playerVelocity);
	EntityManager::AddComponent(playerEntity, playerHealth);
	EntityManager::AddComponent(playerEntity, playerAnimation);

	return playerEntity;
}


Entity EntityManager::CreateEnemyEntity(EntityManager& entityManager, const glm::vec3& playerPos, std::shared_ptr<CSimpleSprite> enemySprite, float screenWidth, float screenHeight)
{
	Entity enemyEntity = entityManager.CreateEntity();
	float minVx = -0.1f, maxVx = 0.3f;
	float minVy = -0.1f, maxVy = 0.3f;
	glm::vec3 enemyPos = Helper::GetOppositeQuadrantPosition(playerPos, 1024.0f, 768.0f);
	glm::vec2 randomVelocity = Helper::GenerateVec2(minVx, maxVx, minVy, maxVy);
	float enemyScale = 0.5f;
	SpriteDimensions enemyDimensions = Helper::GetSpriteDimensions(enemySprite, 1.0f);

	auto enemyTag = std::make_shared<Tag>(EntityType::ENEMY);
	auto enemyTransform = std::make_shared<Transform>(enemyPos, glm::vec3(0.0f), glm::vec3(enemyScale));
	auto enemyRenderable = std::make_shared<Renderable>(enemySprite);
	auto enemyCollider = std::make_shared<Collider>();
	enemyCollider->collisionShape = CollisionShape::SPHERE;
	enemyCollider->collisionType = CollisionType::ENEMY;
	enemyCollider->collisionMask = static_cast<int>(CollisionType::PLAYER) | static_cast<int>(CollisionType::BULLET);
	enemyCollider->radius = enemyDimensions.width / 2;
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

Entity EntityManager::CreateBulletEntity(std::shared_ptr<CSimpleSprite> bulletSprite, const glm::vec3& position, const glm::vec2& velocity)
{
	Entity bulletEntity = CreateEntity();
	float bulletScale = 2.0f;
	SpriteDimensions bulletDimensions = Helper::GetSpriteDimensions(bulletSprite, 1.0f);

	auto bulletTag = std::make_shared<Tag>(EntityType::BULLET);
	auto bulletTransform = std::make_shared<Transform>(position, glm::vec3(0.0f), glm::vec3(bulletScale));
	auto bulletRenderable = std::make_shared<Renderable>(bulletSprite);
	auto bulletCollider = std::make_shared<Collider>();
	bulletCollider->collisionShape = CollisionShape::SPHERE;
	bulletCollider->collisionType = CollisionType::BULLET;
	bulletCollider->collisionMask = static_cast<int>(CollisionType::ENEMY);
	bulletCollider->radius = bulletDimensions.width / 2;
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

void EntityManager::MarkEntityForDeletion(Entity entity)
{
	// Check if already marked for deletion
	if (std::find(entitiesToDelete.begin(), entitiesToDelete.end(), entity) == entitiesToDelete.end()) {
		entitiesToDelete.push_back(entity);
	}
}

void EntityManager::ProcessDeletions() {
	for (Entity entity : entitiesToDelete) {
		auto tag = GetComponent<Tag>(entity);
		if (tag->entityType == EntityType::ENEMY) {
			auto enemyAnimation = GetComponent<Animation>(entity);
			if (enemyAnimation && enemyAnimation->cooldownTimer > 0.0f) {
				continue;
			}
			entityComponents.erase(entity);
		}
		else
		{
			entityComponents.erase(entity);
		}
	}
	entitiesToDelete.clear();
}

void EntityManager::ProcessBulletHitEnemy(EntityManager& entityManager, float deltaTime, SystemEvent event, const glm::vec3& playerPos, float screenWidth, float screenHeight)
{
	CSimpleSprite* rawEnemySprite1 = App::CreateSprite(".\\Data\\Sprites\\Enemy.png", 4, 2);
	std::shared_ptr<CSimpleSprite> enemySprite1 = std::shared_ptr<CSimpleSprite>(rawEnemySprite1);

	CSimpleSprite* rawEnemySprite2 = App::CreateSprite(".\\Data\\Sprites\\Enemy.png", 4, 2);
	std::shared_ptr<CSimpleSprite> enemySprite2 = std::shared_ptr<CSimpleSprite>(rawEnemySprite2);

	CreateEnemyEntity(entityManager, playerPos, enemySprite1, screenWidth, screenHeight);
	CreateEnemyEntity(entityManager, playerPos, enemySprite2, screenWidth, screenHeight);
}
