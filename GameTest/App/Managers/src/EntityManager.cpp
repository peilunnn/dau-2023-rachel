#include "stdafx.h"
#include "../include/EntityManager.h"
#include "../../Systems/include/AnimationHandler.h"

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
	float minVx = -100.0f, maxVx = 300.0f;
	float minVy = -100.0, maxVy = 300.0f;
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
	auto enemyAnimation = std::make_shared<Animation>();

	EntityManager::AddComponent(enemyEntity, enemyTag);
	EntityManager::AddComponent(enemyEntity, enemyTransform);
	EntityManager::AddComponent(enemyEntity, enemyRenderable);
	EntityManager::AddComponent(enemyEntity, enemyCollider);
	EntityManager::AddComponent(enemyEntity, enemyVelocity);
	EntityManager::AddComponent(enemyEntity, enemyDirection);
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

	AddComponent(bulletEntity, bulletTag);
	AddComponent(bulletEntity, bulletTransform);
	AddComponent(bulletEntity, bulletRenderable);
	AddComponent(bulletEntity, bulletCollider);
	AddComponent(bulletEntity, bulletVelocity);

	return bulletEntity;
}

Entity EntityManager::CreateReloadingCircleEntity(std::shared_ptr<CSimpleSprite> reloadingCircleSprite)
{
	Entity reloadingCircleEntity = CreateEntity();
	float reloadingCircleScale = 0.4f;
	float maxX = 800.0f;
	float maxY = 400.0f;
	float reloadingCirclePosX = Helper::GenerateFloat(0.0, maxX);
	float reloadingCirclePosY = Helper::GenerateFloat(0.0, maxY);
	float reloadingCirclePosZ = 0.0f;
	SpriteDimensions reloadingCircleDimensions = Helper::GetSpriteDimensions(reloadingCircleSprite, 1.0f);

	auto reloadingCircleTag = std::make_shared<Tag>(EntityType::RELOADING_CIRCLE);
	auto reloadingCircleTransform = std::make_shared<Transform>(glm::vec3(reloadingCirclePosX, reloadingCirclePosY, reloadingCirclePosZ), glm::vec3(0.0f), glm::vec3(reloadingCircleScale));
	auto reloadingCircleRenderable = std::make_shared<Renderable>(reloadingCircleSprite);
	auto reloadingCircleCollider = std::make_shared<Collider>();
	reloadingCircleCollider->collisionShape = CollisionShape::SPHERE;
	reloadingCircleCollider->collisionType = CollisionType::BULLET;
	reloadingCircleCollider->collisionMask = static_cast<int>(CollisionType::PLAYER);
	reloadingCircleCollider->radius = reloadingCircleDimensions.width / 2;
	auto reloadingCircleAnimation = std::make_shared<Animation>();

	AddComponent(reloadingCircleEntity, reloadingCircleTag);
	AddComponent(reloadingCircleEntity, reloadingCircleTransform);
	AddComponent(reloadingCircleEntity, reloadingCircleRenderable);
	AddComponent(reloadingCircleEntity, reloadingCircleCollider);
	AddComponent(reloadingCircleEntity, reloadingCircleAnimation);

	return reloadingCircleEntity;
}

Entity EntityManager::CreateAmmoEmptyEntity(std::shared_ptr<CSimpleSprite> ammoEmptySprite, float xPos, float yPos)
{
	Entity ammoEmptyEntity = CreateEntity();
	float zPos = 0.0f;
	float ammoEmptyScale = 0.5f;

	auto ammoEmptyTag = std::make_shared<Tag>(EntityType::AMMO_EMPTY);
	auto ammoEmptyTransform = std::make_shared<Transform>(glm::vec3(xPos, yPos, zPos), glm::vec3(0.0f), glm::vec3(ammoEmptyScale));
	auto ammoEmptyRenderable = std::make_shared<Renderable>(ammoEmptySprite);

	AddComponent(ammoEmptyEntity, ammoEmptyTag);
	AddComponent(ammoEmptyEntity, ammoEmptyTransform);
	AddComponent(ammoEmptyEntity, ammoEmptyRenderable);

	return ammoEmptyEntity;
}

Entity EntityManager::CreateAmmoFilledEntity(std::shared_ptr<CSimpleSprite> ammoFilledSprite, float xPos, float yPos)
{
	Entity ammoFilledEntity = CreateEntity();
	float zPos = 0.0f;
	float ammoFilledScale = 0.5f;

	auto ammoFilledTag = std::make_shared<Tag>(EntityType::AMMO_FILLED);
	auto ammoFilledTransform = std::make_shared<Transform>(glm::vec3(xPos, yPos, zPos), glm::vec3(0.0f), glm::vec3(ammoFilledScale));
	auto ammoFilledRenderable = std::make_shared<Renderable>(ammoFilledSprite);

	AddComponent(ammoFilledEntity, ammoFilledTag);
	AddComponent(ammoFilledEntity, ammoFilledTransform);
	AddComponent(ammoFilledEntity, ammoFilledRenderable);

	return ammoFilledEntity;
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
				Helper::Log("still in cooldown, can't delete enemy");
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

void EntityManager::ProcessBulletHitEnemy(EntityManager& entityManager, float deltaTime, Event event, const glm::vec3& playerPos, float screenWidth, float screenHeight)
{
	CSimpleSprite* rawEnemySprite1 = App::CreateSprite(Helper::pathToEnemySpriteSheet, 4, 2);
	std::shared_ptr<CSimpleSprite> enemySprite1 = std::shared_ptr<CSimpleSprite>(rawEnemySprite1);

	CSimpleSprite* rawEnemySprite2 = App::CreateSprite(Helper::pathToEnemySpriteSheet, 4, 2);
	std::shared_ptr<CSimpleSprite> enemySprite2 = std::shared_ptr<CSimpleSprite>(rawEnemySprite2);

	Entity enemyEntity1 = CreateEnemyEntity(entityManager, playerPos, enemySprite1, screenWidth, screenHeight);
	AnimationHandler::InitEnemyAnimation(enemySprite1);

	Entity enemyEntity2 = CreateEnemyEntity(entityManager, playerPos, enemySprite2, screenWidth, screenHeight);
	AnimationHandler::InitEnemyAnimation(enemySprite2);
}
