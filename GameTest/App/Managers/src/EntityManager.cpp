#include "stdafx.h"
#include "../../Components/include/Animation.h"
#include "../../Components/include/Collider.h"
#include "../../Components/include/Direction.h"
#include "../../Components/include/EntityId.h"
#include "../../Components/include/Health.h"
#include "../../Components/include/Renderable.h"
#include "../../Components/include/Score.h"
#include "../../Components/include/Tag.h"
#include "../../Components/include/Timer.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Velocity.h"
#include "../../Managers/include/EntityManager.h"
#include "../../Managers/include/SpriteManager.h"
#include "../../Systems/include/AnimationHandler.h"
#include "../../Systems/include/Event.h"
#include "../../Systems/include/ScreenHandler.h"
#include "../../Systems/include/ShootingHandler.h"
#include "../../Utilities/include/App.h"
#include "../../Utilities/include/Helper.h"

EntityId EntityManager::nextEntityId = 0;

void EntityManager::Init(shared_ptr<CSimpleSprite> playerSprite, shared_ptr<CSimpleSprite> enemySprite, shared_ptr<CSimpleSprite> reloadingCircleSprite, shared_ptr<CSimpleSprite> ammoEmptySprite, shared_ptr<CSimpleSprite> ammoFilledSprite, shared_ptr<CSimpleSprite> healthBarSprite)
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
		shared_ptr<CSimpleSprite> ammoEmptySprite = shared_ptr<CSimpleSprite>(rawAmmoEmptySprite);
		CSimpleSprite* rawAmmoFilledSprite = App::CreateSprite(Helper::PATH_TO_AMMO_FILLED_SPRITE, 1, 1);
		shared_ptr<CSimpleSprite> ammoFilledSprite = shared_ptr<CSimpleSprite>(rawAmmoFilledSprite);

		//shared_ptr<CSimpleSprite> ammoEmptySprite = SpriteManager::CreateSprite(Helper::PATH_TO_AMMO_EMPTY_SPRITE, 1, 1);
		//shared_ptr<CSimpleSprite> ammoFilledSprite = SpriteManager::CreateSprite(Helper::PATH_TO_AMMO_FILLED_SPRITE, 1, 1);

		ammoEmptyEntity = CreateAmmoEntity(ammoEmptySprite, EntityType::AMMO_EMPTY, xPos, ammoYPos);
		ammoEmptyEntities.push_back(ammoEmptyEntity);
		ammoFilledEntity = CreateAmmoEntity(ammoFilledSprite, EntityType::AMMO_FILLED, xPos, ammoYPos);
		ammoFilledEntities.push_back(ammoFilledEntity);
	}

	scoreEntity = CreateScoreEntity();
	timerEntity = CreateTimerEntity();
}

vector<EntityId> EntityManager::GetAllEntities()
{
	vector<EntityId> allEntities;
	for (const auto& pair : entityComponents) {
		allEntities.push_back(pair.first);
	}
	return allEntities;
}

EntityId EntityManager::CreateEntity()
{
	return nextEntityId++;
}

EntityId EntityManager::CreatePlayerEntity(shared_ptr<CSimpleSprite> playerSprite)
{
	EntityId playerEntity = CreateEntity();
	float maxX = ScreenHandler::SCREEN_RIGHT - ScreenHandler::SCREEN_LEFT;
	float maxY = ScreenHandler::SCREEN_BOTTOM - ScreenHandler::SCREEN_TOP;
	float xPos = Helper::GenerateFloat(ScreenHandler::SCREEN_LEFT, ScreenHandler::SCREEN_RIGHT);
	float yPos = Helper::GenerateFloat(ScreenHandler::SCREEN_TOP, ScreenHandler::SCREEN_BOTTOM);
	float zPos = 0.0f;
	float scale = 0.75f;
	SpriteDimensions dimensions = Helper::GetSpriteDimensions(playerSprite, 1.0f);

	auto tag = make_shared<Tag>(EntityType::PLAYER);
	auto transform = make_shared<Transform>(glm::vec3(xPos, yPos, zPos), glm::vec3(0.0f), glm::vec3(scale));
	auto renderable = make_shared<Renderable>(playerSprite);
	auto collider = make_shared<Collider>();
	collider->collisionShape = CollisionShape::CAPSULE;
	collider->collisionType = CollisionType::PLAYER;
	collider->collisionMask = static_cast<int>(CollisionType::ENEMY) | static_cast<int>(CollisionType::RELOADING_CIRCLE);
	collider->radius = dimensions.height / 2;
	auto velocity = make_shared<Velocity>(glm::vec2(0.0f));
	auto health = make_shared<Health>();
	auto animation = make_shared<Animation>();

	EntityManager::AddComponent(playerEntity, tag);
	EntityManager::AddComponent(playerEntity, transform);
	EntityManager::AddComponent(playerEntity, renderable);
	EntityManager::AddComponent(playerEntity, collider);
	EntityManager::AddComponent(playerEntity, velocity);
	EntityManager::AddComponent(playerEntity, health);
	EntityManager::AddComponent(playerEntity, animation);

	return playerEntity;
}

EntityId EntityManager::CreateEnemyEntity(const glm::vec3& playerPos, shared_ptr<CSimpleSprite> enemySprite, float screenWidth, float screenHeight)
{
	EntityId enemyEntity = CreateEntity();
	float minVx = -100.0f, maxVx = 300.0f;
	float minVy = -100.0, maxVy = 300.0f;
	glm::vec3 pos = Helper::GetOppositeQuadrantPosition(playerPos, 1024.0f, 768.0f);
	glm::vec2 randomVelocity = Helper::GenerateVec2(minVx, maxVx, minVy, maxVy);
	float scale = 0.4f;
	SpriteDimensions dimensions = Helper::GetSpriteDimensions(enemySprite, 1.0f);

	auto tag = make_shared<Tag>(EntityType::ENEMY);
	auto transform = make_shared<Transform>(pos, glm::vec3(0.0f), glm::vec3(scale));
	auto renderable = make_shared<Renderable>(enemySprite);
	auto collider = make_shared<Collider>();
	collider->collisionShape = CollisionShape::SPHERE;
	collider->collisionType = CollisionType::ENEMY;
	collider->collisionMask = static_cast<int>(CollisionType::PLAYER) | static_cast<int>(CollisionType::BULLET);
	collider->radius = dimensions.width / 2;
	auto velocity = make_shared<Velocity>(randomVelocity);
	auto direction = make_shared<Direction>();
	auto animation = make_shared<Animation>();

	EntityManager::AddComponent(enemyEntity, tag);
	EntityManager::AddComponent(enemyEntity, transform);
	EntityManager::AddComponent(enemyEntity, renderable);
	EntityManager::AddComponent(enemyEntity, collider);
	EntityManager::AddComponent(enemyEntity, velocity);
	EntityManager::AddComponent(enemyEntity, direction);
	EntityManager::AddComponent(enemyEntity, animation);

	return enemyEntity;
}

EntityId EntityManager::CreateBulletEntity(shared_ptr<CSimpleSprite> bulletSprite, const glm::vec3& position, const glm::vec2& targetVelocity)
{
	EntityId bulletEntity = CreateEntity();
	float scale = 1.0f;
	SpriteDimensions dimensions = Helper::GetSpriteDimensions(bulletSprite, 1.0f);

	auto tag = make_shared<Tag>(EntityType::BULLET);
	auto transform = make_shared<Transform>(position, glm::vec3(0.0f), glm::vec3(scale));
	auto renderable = make_shared<Renderable>(bulletSprite);
	auto collider = make_shared<Collider>();
	collider->collisionShape = CollisionShape::SPHERE;
	collider->collisionType = CollisionType::BULLET;
	collider->collisionMask = static_cast<int>(CollisionType::ENEMY);
	collider->radius = dimensions.width / 2;
	auto velocity = make_shared<Velocity>(targetVelocity);

	AddComponent(bulletEntity, tag);
	AddComponent(bulletEntity, transform);
	AddComponent(bulletEntity, renderable);
	AddComponent(bulletEntity, collider);
	AddComponent(bulletEntity, velocity);

	return bulletEntity;
}

EntityId EntityManager::CreateReloadingCircleEntity(shared_ptr<CSimpleSprite> reloadingCircleSprite)
{
	EntityId reloadingCircleEntity = CreateEntity();
	float maxX = ScreenHandler::SCREEN_RIGHT - ScreenHandler::SCREEN_LEFT;
	float maxY = ScreenHandler::SCREEN_BOTTOM - ScreenHandler::SCREEN_TOP;
	float xPos = Helper::GenerateFloat(ScreenHandler::SCREEN_LEFT, ScreenHandler::SCREEN_RIGHT);
	float yPos = Helper::GenerateFloat(ScreenHandler::SCREEN_TOP, ScreenHandler::SCREEN_BOTTOM);
	float zPos = 0.0f;
	float scale = 0.4f;
	SpriteDimensions dimensions = Helper::GetSpriteDimensions(reloadingCircleSprite, 1.0f);

	auto tag = make_shared<Tag>(EntityType::RELOADING_CIRCLE);
	auto transform = make_shared<Transform>(glm::vec3(xPos, yPos, zPos), glm::vec3(0.0f), glm::vec3(scale));
	auto renderable = make_shared<Renderable>(reloadingCircleSprite);
	auto collider = make_shared<Collider>();
	collider->collisionShape = CollisionShape::SPHERE;
	collider->collisionType = CollisionType::RELOADING_CIRCLE;
	collider->collisionMask = static_cast<int>(CollisionType::PLAYER);
	collider->radius = dimensions.width / 2;
	auto animation = make_shared<Animation>();

	AddComponent(reloadingCircleEntity, tag);
	AddComponent(reloadingCircleEntity, transform);
	AddComponent(reloadingCircleEntity, renderable);
	AddComponent(reloadingCircleEntity, collider);
	AddComponent(reloadingCircleEntity, animation);

	return reloadingCircleEntity;
}

EntityId EntityManager::CreateAmmoEntity(shared_ptr<CSimpleSprite> sprite, EntityType entityType, float xPos, float yPos)
{
	EntityId ammoEntity = CreateEntity();
	float zPos = 0.0f;
	float scale = 0.5f;

	auto tag = make_shared<Tag>(entityType);
	auto transform = make_shared<Transform>(glm::vec3(xPos, yPos, zPos), glm::vec3(0.0f), glm::vec3(scale));
	auto renderable = make_shared<Renderable>(sprite);

	AddComponent(ammoEntity, tag);
	AddComponent(ammoEntity, transform);
	AddComponent(ammoEntity, renderable);

	return ammoEntity;
}

EntityId EntityManager::CreateHealthBarEntity(shared_ptr<CSimpleSprite> sprite, float xPos, float yPos)
{
	EntityId healthBarEntity = CreateEntity();
	float zPos = 0.0f;

	auto tag = make_shared<Tag>(EntityType::HEALTH_BAR);
	auto transform = make_shared<Transform>(glm::vec3(xPos, yPos, zPos), glm::vec3(0.0f), glm::vec3(1.0f));
	auto renderable = make_shared<Renderable>(sprite);
	auto animation = make_shared<Animation>();

	AddComponent(healthBarEntity, tag);
	AddComponent(healthBarEntity, transform);
	AddComponent(healthBarEntity, renderable);
	AddComponent(healthBarEntity, animation);

	return healthBarEntity;
}

EntityId EntityManager::CreateScoreEntity()
{
	EntityId scoreEntity = CreateEntity();
	float yOffset = 50.0f;
	float xPos = ScreenHandler::SCREEN_WIDTH / 2;
	float yPos = ScreenHandler::SCREEN_HEIGHT - yOffset;
	float zPos = 0.0f;

	auto tag = make_shared<Tag>(EntityType::SCORE);
	auto transform = make_shared<Transform>(glm::vec3(xPos, yPos, zPos), glm::vec3(0.0f), glm::vec3(1.0f));
	auto score = make_shared<Score>();

	AddComponent(scoreEntity, tag);
	AddComponent(scoreEntity, transform);
	AddComponent(scoreEntity, score);

	return scoreEntity;
}

EntityId EntityManager::CreateTimerEntity()
{
	EntityId timerEntity = CreateEntity();
	float xOffset = 1000.0f;
	float yOffset = 50.0f;
	float xPos = ScreenHandler::SCREEN_WIDTH - xOffset;
	float yPos = ScreenHandler::SCREEN_HEIGHT - yOffset;
	float zPos = 0.0f;

	auto tag = make_shared<Tag>(EntityType::TIMER);
	auto transform = make_shared<Transform>(glm::vec3(xPos, yPos, zPos), glm::vec3(0.0f), glm::vec3(1.0f));
	auto timer = make_shared<Timer>();

	AddComponent(timerEntity, tag);
	AddComponent(timerEntity, transform);
	AddComponent(timerEntity, timer);

	return timerEntity;
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

void EntityManager::MoveEntityToRandomPos(EntityId entityId)
{
	float xPos = Helper::GenerateFloat(ScreenHandler::SCREEN_LEFT, ScreenHandler::SCREEN_RIGHT);
	float yPos = Helper::GenerateFloat(ScreenHandler::SCREEN_TOP, ScreenHandler::SCREEN_BOTTOM);
	float zPos = 0.0f;

	glm::vec3 newPos = glm::vec3(xPos, yPos, zPos);
	auto transform = GetComponent<Transform>(entityId);
	transform->position = newPos;
}

void EntityManager::MarkEntityForDeletion(EntityId entityId)
{
	// Check if already marked for deletion
	if (find(entitiesToDelete.begin(), entitiesToDelete.end(), entityId) == entitiesToDelete.end()) {
		entitiesToDelete.push_back(entityId);
	}
}

void EntityManager::ProcessDeletions() {
	for (EntityId entityId : entitiesToDelete) {
		auto tag = GetComponent<Tag>(entityId);
		if (tag->entityType == EntityType::ENEMY) {
			auto enemyAnimation = GetComponent<Animation>(entityId);
			if (enemyAnimation && enemyAnimation->cooldownTimer > 0.0f)
				continue;
			entityComponents.erase(entityId);
		}
		else
			entityComponents.erase(entityId);
	}
	entitiesToDelete.clear();
}

void EntityManager::ProcessBulletHitEnemy(EntityManager& entityManager, Event event, float deltaTime, const glm::vec3& playerPos, float screenWidth, float screenHeight)
{
	CSimpleSprite* rawEnemySprite1 = App::CreateSprite(Helper::PATH_TO_ENEMY_SPRITE_SHEET, 4, 2);
	shared_ptr<CSimpleSprite> enemySprite1 = shared_ptr<CSimpleSprite>(rawEnemySprite1);
	CSimpleSprite* rawEnemySprite2 = App::CreateSprite(Helper::PATH_TO_ENEMY_SPRITE_SHEET, 4, 2);
	shared_ptr<CSimpleSprite> enemySprite2 = shared_ptr<CSimpleSprite>(rawEnemySprite2);

	//shared_ptr<CSimpleSprite> enemySprite1 = SpriteManager::CreateSprite(Helper::PATH_TO_ENEMY_SPRITE_SHEET, 4, 2);
	//shared_ptr<CSimpleSprite> enemySprite2 = SpriteManager::CreateSprite(Helper::PATH_TO_ENEMY_SPRITE_SHEET, 4, 2);

	EntityId enemyEntity1 = CreateEnemyEntity(playerPos, enemySprite1, screenWidth, screenHeight);
	AnimationHandler::InitEnemyAnimation(enemySprite1);
	EntityId enemyEntity2 = CreateEnemyEntity(playerPos, enemySprite2, screenWidth, screenHeight);
	AnimationHandler::InitEnemyAnimation(enemySprite2);
}

void EntityManager::ProcessEnemyHitPlayer(EntityManager& entityManager, Event event, float deltaTime)
{
	EntityId playerEntity, enemyEntity;
	EntityId entity1Id = event.entities[0];
	EntityId entity2Id = event.entities[1];

	auto tag1 = entityManager.GetComponent<Tag>(entity1Id);
	auto tag2 = entityManager.GetComponent<Tag>(entity2Id);

	if (!tag1 || !tag2)
		return;

	if (tag1->entityType == EntityType::PLAYER) {
		playerEntity = entity1Id;
		enemyEntity = entity2Id;
	}
	else {
		playerEntity = entity2Id;
		enemyEntity = entity1Id;
	}

	MarkEntityForDeletion(enemyEntity);
}
