#include "stdafx.h"
#include "../include/EntityManager.h"
#include "../../Systems/include/AnimationHandler.h"
#include "../../Systems/include/ShootingHandler.h"

Entity EntityManager::nextEntityId = 0;

void EntityManager::Init(std::shared_ptr<CSimpleSprite> playerSprite, std::shared_ptr<CSimpleSprite> enemySprite, std::shared_ptr<CSimpleSprite> reloadingCircleSprite, std::shared_ptr<CSimpleSprite> ammoEmptySprite, std::shared_ptr<CSimpleSprite> ammoFilledSprite, float screenWidth, float screenHeight, float startingX, float yPos, float ammoSpriteSpacing, int maxBullets)
{
	playerEntity = CreatePlayerEntity(playerSprite);
	glm::vec3 playerPos = GetComponent<Transform>(playerEntity)->position;
	enemyEntity = CreateEnemyEntity(playerPos, enemySprite, screenWidth, screenHeight);
	reloadingCircleEntity = CreateReloadingCircleEntity(reloadingCircleSprite);

	for (int i = 0; i < maxBullets; ++i)
	{
		float xPos = startingX - i * ammoSpriteSpacing;
		
		// Have to create a new sprite for every entity
		CSimpleSprite* rawAmmoEmptySprite = App::CreateSprite(Helper::pathToAmmoEmptySprite, 1, 1);
		std::shared_ptr<CSimpleSprite> ammoEmptySprite = std::shared_ptr<CSimpleSprite>(rawAmmoEmptySprite);
		CSimpleSprite* rawAmmoFilledSprite = App::CreateSprite(Helper::pathToAmmoFilledSprite, 1, 1);
		std::shared_ptr<CSimpleSprite> ammoFilledSprite = std::shared_ptr<CSimpleSprite>(rawAmmoFilledSprite);

		ammoEmptyEntity = CreateAmmoEntity(ammoEmptySprite, EntityType::AMMO_EMPTY, xPos, yPos);
		ammoEmptyEntities.push_back(ammoEmptyEntity);
		ammoFilledEntity = CreateAmmoEntity(ammoFilledSprite, EntityType::AMMO_FILLED, xPos, yPos);
		ammoFilledEntities.push_back(ammoFilledEntity);
	}
}

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
	Entity player = CreateEntity();
	float maxX = 800.0f;
	float maxY = 400.0f;
	float xPos = Helper::GenerateFloat(0.0, maxX);
	float yPos = Helper::GenerateFloat(0.0, maxY);
	float zPos = 0.0f;
	SpriteDimensions dimensions = Helper::GetSpriteDimensions(playerSprite, 1.0f);

	auto tag = std::make_shared<Tag>(EntityType::PLAYER);
	auto transform = std::make_shared<Transform>(glm::vec3(xPos, yPos, zPos), glm::vec3(0.0f), glm::vec3(1.0f));
	auto renderable = std::make_shared<Renderable>(playerSprite);
	auto collider = std::make_shared<Collider>();
	collider->collisionShape = CollisionShape::CAPSULE;
	collider->collisionType = CollisionType::PLAYER;
	collider->collisionMask = static_cast<int>(CollisionType::ENEMY);
	collider->radius = dimensions.height / 2;
	auto velocity = std::make_shared<Velocity>(0.0f, 0.0f);
	auto health = std::make_shared<Health>();
	auto animation = std::make_shared<Animation>();

	EntityManager::AddComponent(player, tag);
	EntityManager::AddComponent(player, transform);
	EntityManager::AddComponent(player, renderable);
	EntityManager::AddComponent(player, collider);
	EntityManager::AddComponent(player, velocity);
	EntityManager::AddComponent(player, health);
	EntityManager::AddComponent(player, animation);

	return player;
}

Entity EntityManager::CreateEnemyEntity(const glm::vec3& playerPos, std::shared_ptr<CSimpleSprite> enemySprite, float screenWidth, float screenHeight)
{
	Entity enemy = CreateEntity();
	float minVx = -100.0f, maxVx = 300.0f;
	float minVy = -100.0, maxVy = 300.0f;
	glm::vec3 pos = Helper::GetOppositeQuadrantPosition(playerPos, 1024.0f, 768.0f);
	glm::vec2 randomVelocity = Helper::GenerateVec2(minVx, maxVx, minVy, maxVy);
	float scale = 0.5f;
	SpriteDimensions dimensions = Helper::GetSpriteDimensions(enemySprite, 1.0f);

	auto tag = std::make_shared<Tag>(EntityType::ENEMY);
	auto transform = std::make_shared<Transform>(pos, glm::vec3(0.0f), glm::vec3(scale));
	auto renderable = std::make_shared<Renderable>(enemySprite);
	auto collider = std::make_shared<Collider>();
	collider->collisionShape = CollisionShape::SPHERE;
	collider->collisionType = CollisionType::ENEMY;
	collider->collisionMask = static_cast<int>(CollisionType::PLAYER) | static_cast<int>(CollisionType::BULLET);
	collider->radius = dimensions.width / 2;
	auto velocity = std::make_shared<Velocity>(randomVelocity.x, randomVelocity.y);
	auto direction = std::make_shared<Direction>();
	auto animation = std::make_shared<Animation>();

	EntityManager::AddComponent(enemy, tag);
	EntityManager::AddComponent(enemy, transform);
	EntityManager::AddComponent(enemy, renderable);
	EntityManager::AddComponent(enemy, collider);
	EntityManager::AddComponent(enemy, velocity);
	EntityManager::AddComponent(enemy, direction);
	EntityManager::AddComponent(enemy, animation);

	return enemy;
}

Entity EntityManager::CreateBulletEntity(std::shared_ptr<CSimpleSprite> bulletSprite, const glm::vec3& position, const glm::vec2& targetVelocity)
{
	Entity bullet = CreateEntity();
	float scale = 2.0f;
	SpriteDimensions dimensions = Helper::GetSpriteDimensions(bulletSprite, 1.0f);

	auto tag = std::make_shared<Tag>(EntityType::BULLET);
	auto transform = std::make_shared<Transform>(position, glm::vec3(0.0f), glm::vec3(scale));
	auto renderable = std::make_shared<Renderable>(bulletSprite);
	auto collider = std::make_shared<Collider>();
	collider->collisionShape = CollisionShape::SPHERE;
	collider->collisionType = CollisionType::BULLET;
	collider->collisionMask = static_cast<int>(CollisionType::ENEMY);
	collider->radius = dimensions.width / 2;
	auto velocity = std::make_shared<Velocity>(targetVelocity.x, targetVelocity.y);

	AddComponent(bullet, tag);
	AddComponent(bullet, transform);
	AddComponent(bullet, renderable);
	AddComponent(bullet, collider);
	AddComponent(bullet, velocity);

	return bullet;
}

Entity EntityManager::CreateReloadingCircleEntity(std::shared_ptr<CSimpleSprite> reloadingCircleSprite)
{
	Entity reloadingCircle = CreateEntity();
	float scale = 0.4f;
	float maxX = 800.0f;
	float maxY = 400.0f;
	float xPos = Helper::GenerateFloat(0.0, maxX);
	float yPos = Helper::GenerateFloat(0.0, maxY);
	float zPos = 0.0f;
	SpriteDimensions dimensions = Helper::GetSpriteDimensions(reloadingCircleSprite, 1.0f);

	auto tag = std::make_shared<Tag>(EntityType::RELOADING_CIRCLE);
	auto transform = std::make_shared<Transform>(glm::vec3(xPos, yPos, zPos), glm::vec3(0.0f), glm::vec3(scale));
	auto renderable = std::make_shared<Renderable>(reloadingCircleSprite);
	auto collider = std::make_shared<Collider>();
	collider->collisionShape = CollisionShape::SPHERE;
	collider->collisionType = CollisionType::BULLET;
	collider->collisionMask = static_cast<int>(CollisionType::PLAYER);
	collider->radius = dimensions.width / 2;
	auto animation = std::make_shared<Animation>();

	AddComponent(reloadingCircle, tag);
	AddComponent(reloadingCircle, transform);
	AddComponent(reloadingCircle, renderable);
	AddComponent(reloadingCircle, collider);
	AddComponent(reloadingCircle, animation);

	return reloadingCircle;
}

Entity EntityManager::CreateAmmoEntity(std::shared_ptr<CSimpleSprite> sprite, EntityType entityType, float xPos, float yPos)
{
	Entity ammoEntity = CreateEntity();
	float zPos = 0.0f;
	float scale = 0.5f;

	auto tag = std::make_shared<Tag>(entityType);
	auto transform = std::make_shared<Transform>(glm::vec3(xPos, yPos, zPos), glm::vec3(0.0f), glm::vec3(scale));
	auto renderable = std::make_shared<Renderable>(sprite);

	AddComponent(ammoEntity, tag);
	AddComponent(ammoEntity, transform);
	AddComponent(ammoEntity, renderable);

	return ammoEntity;
}

void EntityManager::HideAmmoFilledEntity(int index)
{
	if (index >= 0 && index < ammoFilledEntities.size()) {
		auto ammoFilledSprite = GetComponent<Renderable>(ammoFilledEntities[index])->sprite;

		if (!ammoFilledSprite)
			return;

		ammoFilledSprite->SetVisible(false);
	}
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
			if (enemyAnimation && enemyAnimation->cooldownTimer > 0.0f)
				continue;
			entityComponents.erase(entity);
		}
		else
			entityComponents.erase(entity);
	}
	entitiesToDelete.clear();
}

void EntityManager::ProcessBulletHitEnemy(EntityManager& entityManager, float deltaTime, Event event, const glm::vec3& playerPos, float screenWidth, float screenHeight)
{
	CSimpleSprite* rawEnemySprite1 = App::CreateSprite(Helper::pathToEnemySpriteSheet, 4, 2);
	std::shared_ptr<CSimpleSprite> enemySprite1 = std::shared_ptr<CSimpleSprite>(rawEnemySprite1);
	CSimpleSprite* rawEnemySprite2 = App::CreateSprite(Helper::pathToEnemySpriteSheet, 4, 2);
	std::shared_ptr<CSimpleSprite> enemySprite2 = std::shared_ptr<CSimpleSprite>(rawEnemySprite2);

	Entity enemyEntity1 = CreateEnemyEntity(playerPos, enemySprite1, screenWidth, screenHeight);
	AnimationHandler::InitEnemyAnimation(enemySprite1);
	Entity enemyEntity2 = CreateEnemyEntity(playerPos, enemySprite2, screenWidth, screenHeight);
	AnimationHandler::InitEnemyAnimation(enemySprite2);
}
