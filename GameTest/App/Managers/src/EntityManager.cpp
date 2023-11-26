#include "stdafx.h"
#include "../include/EntityManager.h"
#include "../../Systems/include/AnimationHandler.h"
#include "../../Systems/include/ShootingHandler.h"

Entity EntityManager::nextEntityId = 0;

void EntityManager::Init(std::shared_ptr<CSimpleSprite> playerSprite, std::shared_ptr<CSimpleSprite> enemySprite, std::shared_ptr<CSimpleSprite> reloadingCircleSprite, std::shared_ptr<CSimpleSprite> ammoEmptySprite, std::shared_ptr<CSimpleSprite> ammoFilledSprite, std::shared_ptr<CSimpleSprite> healthBarSprite)
{
	float screenWidth = ScreenHandler::SCREEN_WIDTH;
	float screenHeight = ScreenHandler::SCREEN_HEIGHT;
	float ammoSpriteSpacing = ScreenHandler::AMMO_SPRITE_SPACING;
	int maxBullets = ShootingHandler::GetMaxBullets();
	float ammoStartingX = screenWidth - 20;
	float ammoYPos = screenHeight - 40;
	float healthBarXPos = screenWidth - 880;
	float healthBarYPos = screenHeight - 700;

	playerEntity = CreatePlayerEntity(playerSprite);
	glm::vec3 playerPos = GetComponent<Transform>(playerEntity)->position;
	enemyEntity = CreateEnemyEntity(playerPos, enemySprite, screenWidth, screenHeight);
	reloadingCircleEntity = CreateReloadingCircleEntity(reloadingCircleSprite);
	healthBarEntity = CreateHealthBarEntity(healthBarSprite, healthBarXPos, healthBarYPos);

	for (int i = 0; i < maxBullets; ++i)
	{
		float xPos = ammoStartingX - i * ammoSpriteSpacing;
		
		// Have to create a new sprite for every entity
		CSimpleSprite* rawAmmoEmptySprite = App::CreateSprite(Helper::PATH_TO_AMMO_EMPTY_SPRITE, 1, 1);
		std::shared_ptr<CSimpleSprite> ammoEmptySprite = std::shared_ptr<CSimpleSprite>(rawAmmoEmptySprite);
		CSimpleSprite* rawAmmoFilledSprite = App::CreateSprite(Helper::PATH_TO_AMMO_FILLED_SPRITE, 1, 1);
		std::shared_ptr<CSimpleSprite> ammoFilledSprite = std::shared_ptr<CSimpleSprite>(rawAmmoFilledSprite);

		//std::shared_ptr<CSimpleSprite> ammoEmptySprite = SpriteManager::CreateSprite(Helper::PATH_TO_AMMO_EMPTY_SPRITE, 1, 1);
		//std::shared_ptr<CSimpleSprite> ammoFilledSprite = SpriteManager::CreateSprite(Helper::PATH_TO_AMMO_FILLED_SPRITE, 1, 1);

		ammoEmptyEntity = CreateAmmoEntity(ammoEmptySprite, EntityType::AMMO_EMPTY, xPos, ammoYPos);
		ammoEmptyEntities.push_back(ammoEmptyEntity);
		ammoFilledEntity = CreateAmmoEntity(ammoFilledSprite, EntityType::AMMO_FILLED, xPos, ammoYPos);
		ammoFilledEntities.push_back(ammoFilledEntity);
	}

	scoreEntity = CreateScoreEntity();
}

std::vector<Entity> EntityManager::GetAllEntities()
{
	std::vector<Entity> allEntities;
	for (const auto& pair : entityComponents) {
		allEntities.push_back(pair.first);
	}
	return allEntities;
}

Entity EntityManager::CreateEntity()
{
	return nextEntityId++;
}

Entity EntityManager::CreatePlayerEntity(std::shared_ptr<CSimpleSprite> playerSprite)
{
	Entity playerEntity = CreateEntity();
	float maxX = ScreenHandler::SCREEN_RIGHT - ScreenHandler::SCREEN_LEFT;
	float maxY = ScreenHandler::SCREEN_BOTTOM - ScreenHandler::SCREEN_TOP;
	float xPos = Helper::GenerateFloat(ScreenHandler::SCREEN_LEFT, ScreenHandler::SCREEN_RIGHT);
	float yPos = Helper::GenerateFloat(ScreenHandler::SCREEN_TOP, ScreenHandler::SCREEN_BOTTOM);
	float zPos = 0.0f;
	float scale = 0.75f;
	SpriteDimensions dimensions = Helper::GetSpriteDimensions(playerSprite, 1.0f);

	auto tag = std::make_shared<Tag>(EntityType::PLAYER);
	auto transform = std::make_shared<Transform>(glm::vec3(xPos, yPos, zPos), glm::vec3(0.0f), glm::vec3(scale));
	auto renderable = std::make_shared<Renderable>(playerSprite);
	auto collider = std::make_shared<Collider>();
	collider->collisionShape = CollisionShape::CAPSULE;
	collider->collisionType = CollisionType::PLAYER;
	collider->collisionMask = static_cast<int>(CollisionType::ENEMY) | static_cast<int>(CollisionType::RELOADING_CIRCLE);
	collider->radius = dimensions.height / 2;
	auto velocity = std::make_shared<Velocity>(0.0f, 0.0f);
	auto health = std::make_shared<Health>();
	auto animation = std::make_shared<Animation>();

	EntityManager::AddComponent(playerEntity, tag);
	EntityManager::AddComponent(playerEntity, transform);
	EntityManager::AddComponent(playerEntity, renderable);
	EntityManager::AddComponent(playerEntity, collider);
	EntityManager::AddComponent(playerEntity, velocity);
	EntityManager::AddComponent(playerEntity, health);
	EntityManager::AddComponent(playerEntity, animation);

	return playerEntity;
}

Entity EntityManager::CreateEnemyEntity(const glm::vec3& playerPos, std::shared_ptr<CSimpleSprite> enemySprite, float screenWidth, float screenHeight)
{
	Entity enemyEntity = CreateEntity();
	float minVx = -100.0f, maxVx = 300.0f;
	float minVy = -100.0, maxVy = 300.0f;
	glm::vec3 pos = Helper::GetOppositeQuadrantPosition(playerPos, 1024.0f, 768.0f);
	glm::vec2 randomVelocity = Helper::GenerateVec2(minVx, maxVx, minVy, maxVy);
	float scale = 0.4f;
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

	EntityManager::AddComponent(enemyEntity, tag);
	EntityManager::AddComponent(enemyEntity, transform);
	EntityManager::AddComponent(enemyEntity, renderable);
	EntityManager::AddComponent(enemyEntity, collider);
	EntityManager::AddComponent(enemyEntity, velocity);
	EntityManager::AddComponent(enemyEntity, direction);
	EntityManager::AddComponent(enemyEntity, animation);

	return enemyEntity;
}

Entity EntityManager::CreateBulletEntity(std::shared_ptr<CSimpleSprite> bulletSprite, const glm::vec3& position, const glm::vec2& targetVelocity)
{
	Entity bulletEntity = CreateEntity();
	float scale = 1.0f;
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

	AddComponent(bulletEntity, tag);
	AddComponent(bulletEntity, transform);
	AddComponent(bulletEntity, renderable);
	AddComponent(bulletEntity, collider);
	AddComponent(bulletEntity, velocity);

	return bulletEntity;
}

Entity EntityManager::CreateReloadingCircleEntity(std::shared_ptr<CSimpleSprite> reloadingCircleSprite)
{
	Entity reloadingCircleEntity = CreateEntity();
	float maxX = ScreenHandler::SCREEN_RIGHT - ScreenHandler::SCREEN_LEFT;
	float maxY = ScreenHandler::SCREEN_BOTTOM - ScreenHandler::SCREEN_TOP;
	float xPos = Helper::GenerateFloat(ScreenHandler::SCREEN_LEFT, ScreenHandler::SCREEN_RIGHT);
	float yPos = Helper::GenerateFloat(ScreenHandler::SCREEN_TOP, ScreenHandler::SCREEN_BOTTOM);
	float zPos = 0.0f;
	float scale = 0.4f;
	SpriteDimensions dimensions = Helper::GetSpriteDimensions(reloadingCircleSprite, 1.0f);

	auto tag = std::make_shared<Tag>(EntityType::RELOADING_CIRCLE);
	auto transform = std::make_shared<Transform>(glm::vec3(xPos, yPos, zPos), glm::vec3(0.0f), glm::vec3(scale));
	auto renderable = std::make_shared<Renderable>(reloadingCircleSprite);
	auto collider = std::make_shared<Collider>();
	collider->collisionShape = CollisionShape::SPHERE;
	collider->collisionType = CollisionType::RELOADING_CIRCLE;
	collider->collisionMask = static_cast<int>(CollisionType::PLAYER);
	collider->radius = dimensions.width / 2;
	auto animation = std::make_shared<Animation>();

	AddComponent(reloadingCircleEntity, tag);
	AddComponent(reloadingCircleEntity, transform);
	AddComponent(reloadingCircleEntity, renderable);
	AddComponent(reloadingCircleEntity, collider);
	AddComponent(reloadingCircleEntity, animation);

	return reloadingCircleEntity;
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

Entity EntityManager::CreateHealthBarEntity(std::shared_ptr<CSimpleSprite> sprite, float xPos, float yPos)
{
	Entity healthBarEntity = CreateEntity();
	float zPos = 0.0f;

	auto tag = std::make_shared<Tag>(EntityType::HEALTH_BAR);
	auto transform = std::make_shared<Transform>(glm::vec3(xPos, yPos, zPos), glm::vec3(0.0f), glm::vec3(1.0f));
	auto renderable = std::make_shared<Renderable>(sprite);
	auto animation = std::make_shared<Animation>();

	AddComponent(healthBarEntity, tag);
	AddComponent(healthBarEntity, transform);
	AddComponent(healthBarEntity, renderable);
	AddComponent(healthBarEntity, animation);

	return healthBarEntity;
}

Entity EntityManager::CreateScoreEntity()
{
	Entity scoreEntity = CreateEntity();
	int initialScore = 0;
	float xPos = (ScreenHandler::SCREEN_LEFT + ScreenHandler::SCREEN_RIGHT) / 2.0f;
	float yPos = ScreenHandler::SCREEN_TOP - 50.0f;
	float zPos = 0.0f;

	auto tag = std::make_shared<Tag>(EntityType::HEALTH_BAR);
	auto transform = std::make_shared<Transform>(glm::vec3(xPos, yPos, zPos), glm::vec3(0.0f), glm::vec3(1.0f));
	auto score = std::make_shared<Score>(initialScore);

	AddComponent(scoreEntity, tag);
	AddComponent(scoreEntity, transform);
	AddComponent(scoreEntity, score);

	return scoreEntity;
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

void EntityManager::ShowAllAmmoFilledEntity()
{
	for (int i = 0; i < ammoFilledEntities.size(); i++)
	{
		auto ammoFilledSprite = GetComponent<Renderable>(ammoFilledEntities[i])->sprite;
		ammoFilledSprite->SetVisible(true);
	}
}

void EntityManager::MoveEntityToRandomPos(Entity entity)
{
	float xPos = Helper::GenerateFloat(ScreenHandler::SCREEN_LEFT, ScreenHandler::SCREEN_RIGHT);
	float yPos = Helper::GenerateFloat(ScreenHandler::SCREEN_TOP, ScreenHandler::SCREEN_BOTTOM);
	float zPos = 0.0f;

	glm::vec3 newPos = glm::vec3(xPos, yPos, zPos);
	auto transform = GetComponent<Transform>(entity);
	transform->position = newPos;
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

void EntityManager::ProcessBulletHitEnemy(EntityManager& entityManager, Event event, float deltaTime, const glm::vec3& playerPos, float screenWidth, float screenHeight)
{
	CSimpleSprite* rawEnemySprite1 = App::CreateSprite(Helper::PATH_TO_ENEMY_SPRITE_SHEET, 4, 2);
	std::shared_ptr<CSimpleSprite> enemySprite1 = std::shared_ptr<CSimpleSprite>(rawEnemySprite1);
	CSimpleSprite* rawEnemySprite2 = App::CreateSprite(Helper::PATH_TO_ENEMY_SPRITE_SHEET, 4, 2);
	std::shared_ptr<CSimpleSprite> enemySprite2 = std::shared_ptr<CSimpleSprite>(rawEnemySprite2);

	//std::shared_ptr<CSimpleSprite> enemySprite1 = SpriteManager::CreateSprite(Helper::PATH_TO_ENEMY_SPRITE_SHEET, 4, 2);
	//std::shared_ptr<CSimpleSprite> enemySprite2 = SpriteManager::CreateSprite(Helper::PATH_TO_ENEMY_SPRITE_SHEET, 4, 2);

	Entity enemyEntity1 = CreateEnemyEntity(playerPos, enemySprite1, screenWidth, screenHeight);
	AnimationHandler::InitEnemyAnimation(enemySprite1);
	Entity enemyEntity2 = CreateEnemyEntity(playerPos, enemySprite2, screenWidth, screenHeight);
	AnimationHandler::InitEnemyAnimation(enemySprite2);
}

void EntityManager::ProcessEnemyHitPlayer(EntityManager& entityManager, Event event, float deltaTime)
{
	Entity playerEntity, enemyEntity;
	Entity entity1 = event.entities[0];
	Entity entity2 = event.entities[1];
	
	auto tag1 = entityManager.GetComponent<Tag>(entity1);
	auto tag2 = entityManager.GetComponent<Tag>(entity2);

	if (!tag1 || !tag2)
		return;

	if (tag1->entityType == EntityType::PLAYER) {
		playerEntity = entity1;
		enemyEntity = entity2;
	}
	else {
		playerEntity = entity2;
		enemyEntity = entity1;
	}

	MarkEntityForDeletion(enemyEntity);
}
