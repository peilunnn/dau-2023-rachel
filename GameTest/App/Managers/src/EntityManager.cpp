#include "stdafx.h"
#include "Components/include/Animation.h"
#include "Components/include/Collider.h"
#include "Components/include/Direction.h"
#include "Components/include/EntityId.h"
#include "Components/include/Health.h"
#include "Components/include/Renderable.h"
#include "Components/include/Score.h"
#include "Components/include/Tag.h"
#include "Components/include/Timer.h"
#include "Components/include/Transform.h"
#include "Components/include/Velocity.h"
#include "Managers/include/EntityManager.h"
#include "Managers/include/SpriteManager.h"
#include "Systems/include/AnimationHandler.h"
#include "Systems/include/Event.h"
#include "Systems/include/ScreenHandler.h"
#include "Systems/include/ShootingHandler.h"
#include "Utilities/include/App.h"
#include "Utilities/include/Helper.h"

EntityId EntityManager::s_nextEntityId = 0;

void EntityManager::Init(shared_ptr<CSimpleSprite> playerSprite, shared_ptr<CSimpleSprite> enemySprite, shared_ptr<CSimpleSprite> reloadingCircleSprite, shared_ptr<CSimpleSprite> ammoEmptySprite, shared_ptr<CSimpleSprite> ammoFilledSprite, shared_ptr<CSimpleSprite> healthBarSprite)
{
	constexpr float screenWidth = ScreenHandler::SCREEN_WIDTH;
	constexpr float screenHeight = ScreenHandler::SCREEN_HEIGHT;
	constexpr float ammoSpriteSpacing = ScreenHandler::AMMO_SPRITE_SPACING;
	constexpr int maxBullets = ShootingHandler::MAX_BULLETS;
	constexpr float ammoStartingX = screenWidth - 20;
	constexpr float ammoYPos = screenHeight - 40;
	constexpr float healthBarXPos = screenWidth - 880;
	constexpr float healthBarYPos = screenHeight - 700;

	m_playerEntityId = CreatePlayerEntity(playerSprite);
	glm::vec3 playerPos = GetComponent<Transform>(m_playerEntityId)->GetPosition();
	m_enemyEntityId = CreateEnemyEntity(playerPos, enemySprite, screenWidth, screenHeight);
	m_reloadingCircleEntityId = CreateReloadingCircleEntity(reloadingCircleSprite);
	m_healthBarEntityId = CreateHealthBarEntity(healthBarSprite, healthBarXPos, healthBarYPos);

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

		m_ammoEmptyEntityId = CreateAmmoEntity(ammoEmptySprite, EntityType::AMMO_EMPTY, xPos, ammoYPos);
		m_ammoEmptyEntities.push_back(m_ammoEmptyEntityId);
		m_ammoFilledEntityId = CreateAmmoEntity(ammoFilledSprite, EntityType::AMMO_FILLED, xPos, ammoYPos);
		m_ammoFilledEntities.push_back(m_ammoFilledEntityId);
	}

	m_scoreEntityId = CreateScoreEntity();
	m_timerEntityId = CreateTimerEntity();
}

vector<EntityId> EntityManager::GetAllEntities()
{
	vector<EntityId> allEntities;
	for (const auto& pair : m_entityComponents) {
		allEntities.push_back(pair.first);
	}
	return allEntities;
}

EntityId EntityManager::CreateEntityId()
{
	return s_nextEntityId++;
}

EntityId EntityManager::CreatePlayerEntity(shared_ptr<CSimpleSprite> playerSprite)
{
	EntityId playerEntityId = CreateEntityId();
	const float maxX = ScreenHandler::SCREEN_RIGHT - ScreenHandler::SCREEN_LEFT;
	const float maxY = ScreenHandler::SCREEN_BOTTOM - ScreenHandler::SCREEN_TOP;
	float xPos = Helper::GenerateFloat(ScreenHandler::SCREEN_LEFT, ScreenHandler::SCREEN_RIGHT);
	float yPos = Helper::GenerateFloat(ScreenHandler::SCREEN_TOP, ScreenHandler::SCREEN_BOTTOM);
	constexpr float zPos = 0.0f;
	constexpr float scale = 0.75f;
	SpriteDimensions dimensions = Helper::GetSpriteDimensions(playerSprite, 1.0f);

	auto tag = make_shared<Tag>(EntityType::PLAYER);
	auto transform = make_shared<Transform>(glm::vec3(xPos, yPos, zPos), glm::vec3(0.0f), glm::vec3(scale));
	auto renderable = make_shared<Renderable>(playerSprite);
	auto collider = make_shared<Collider>();
	collider->SetCollisionShape(CollisionShape::CAPSULE);
	collider->SetCollisionType(CollisionType::PLAYER);
	collider->SetCollisionMask(static_cast<int>(CollisionType::ENEMY) | static_cast<int>(CollisionType::RELOADING_CIRCLE));
	collider->SetRadius(dimensions.height / 2);
	auto velocity = make_shared<Velocity>(glm::vec2(0.0f));
	auto health = make_shared<Health>();
	auto animation = make_shared<Animation>();

	EntityManager::AddComponent(playerEntityId, tag);
	EntityManager::AddComponent(playerEntityId, transform);
	EntityManager::AddComponent(playerEntityId, renderable);
	EntityManager::AddComponent(playerEntityId, collider);
	EntityManager::AddComponent(playerEntityId, velocity);
	EntityManager::AddComponent(playerEntityId, health);
	EntityManager::AddComponent(playerEntityId, animation);

	return playerEntityId;
}

EntityId EntityManager::CreateEnemyEntity(const glm::vec3& playerPos, shared_ptr<CSimpleSprite> enemySprite, float screenWidth, float screenHeight)
{
	EntityId enemyEntityId = CreateEntityId();
	constexpr float minVx = -100.0f, maxVx = 300.0f;
	constexpr float minVy = -100.0, maxVy = 300.0f;
	glm::vec3 pos = Helper::GetOppositeQuadrantPosition(playerPos, 1024.0f, 768.0f);
	glm::vec2 randomVelocity = Helper::GenerateVec2(minVx, maxVx, minVy, maxVy);
	constexpr float scale = 0.4f;
	SpriteDimensions dimensions = Helper::GetSpriteDimensions(enemySprite, 1.0f);

	auto tag = make_shared<Tag>(EntityType::ENEMY);
	auto transform = make_shared<Transform>(pos, glm::vec3(0.0f), glm::vec3(scale));
	auto renderable = make_shared<Renderable>(enemySprite);
	auto collider = make_shared<Collider>();
	collider->SetCollisionShape(CollisionShape::SPHERE);
	collider->SetCollisionType(CollisionType::ENEMY);
	collider->SetCollisionMask(static_cast<int>(CollisionType::PLAYER) | static_cast<int>(CollisionType::BULLET));
	collider->SetRadius(dimensions.width / 2);
	auto velocity = make_shared<Velocity>(randomVelocity);
	auto direction = make_shared<Direction>();
	auto animation = make_shared<Animation>();

	EntityManager::AddComponent(enemyEntityId, tag);
	EntityManager::AddComponent(enemyEntityId, transform);
	EntityManager::AddComponent(enemyEntityId, renderable);
	EntityManager::AddComponent(enemyEntityId, collider);
	EntityManager::AddComponent(enemyEntityId, velocity);
	EntityManager::AddComponent(enemyEntityId, direction);
	EntityManager::AddComponent(enemyEntityId, animation);

	return enemyEntityId;
}

EntityId EntityManager::CreateBulletEntity(shared_ptr<CSimpleSprite> bulletSprite, const glm::vec3& position, const glm::vec2& targetVelocity)
{
	EntityId bulletEntityId = CreateEntityId();
	constexpr float scale = 1.0f;
	SpriteDimensions dimensions = Helper::GetSpriteDimensions(bulletSprite, 1.0f);

	auto tag = make_shared<Tag>(EntityType::BULLET);
	auto transform = make_shared<Transform>(position, glm::vec3(0.0f), glm::vec3(scale));
	auto renderable = make_shared<Renderable>(bulletSprite);
	auto collider = make_shared<Collider>();
	collider->SetCollisionShape(CollisionShape::SPHERE);
	collider->SetCollisionType(CollisionType::BULLET);
	collider->SetCollisionMask(static_cast<int>(CollisionType::ENEMY));
	collider->SetRadius(dimensions.width / 2);
	auto velocity = make_shared<Velocity>(targetVelocity);

	AddComponent(bulletEntityId, tag);
	AddComponent(bulletEntityId, transform);
	AddComponent(bulletEntityId, renderable);
	AddComponent(bulletEntityId, collider);
	AddComponent(bulletEntityId, velocity);

	return bulletEntityId;
}

EntityId EntityManager::CreateReloadingCircleEntity(shared_ptr<CSimpleSprite> reloadingCircleSprite)
{
	EntityId reloadingCircleEntityId = CreateEntityId();
	const float maxX = ScreenHandler::SCREEN_RIGHT - ScreenHandler::SCREEN_LEFT;
	const float maxY = ScreenHandler::SCREEN_BOTTOM - ScreenHandler::SCREEN_TOP;
	float xPos = Helper::GenerateFloat(ScreenHandler::SCREEN_LEFT, ScreenHandler::SCREEN_RIGHT);
	float yPos = Helper::GenerateFloat(ScreenHandler::SCREEN_TOP, ScreenHandler::SCREEN_BOTTOM);
	constexpr float zPos = 0.0f;
	constexpr float scale = 0.4f;
	SpriteDimensions dimensions = Helper::GetSpriteDimensions(reloadingCircleSprite, 1.0f);

	auto tag = make_shared<Tag>(EntityType::RELOADING_CIRCLE);
	auto transform = make_shared<Transform>(glm::vec3(xPos, yPos, zPos), glm::vec3(0.0f), glm::vec3(scale));
	auto renderable = make_shared<Renderable>(reloadingCircleSprite);
	auto collider = make_shared<Collider>();
	collider->SetCollisionShape(CollisionShape::SPHERE);
	collider->SetCollisionType(CollisionType::RELOADING_CIRCLE);
	collider->SetCollisionMask(static_cast<int>(CollisionType::PLAYER));
	collider->SetRadius(dimensions.width / 2.0f);
	auto animation = make_shared<Animation>();

	AddComponent(reloadingCircleEntityId, tag);
	AddComponent(reloadingCircleEntityId, transform);
	AddComponent(reloadingCircleEntityId, renderable);
	AddComponent(reloadingCircleEntityId, collider);
	AddComponent(reloadingCircleEntityId, animation);

	return reloadingCircleEntityId;
}

EntityId EntityManager::CreateAmmoEntity(shared_ptr<CSimpleSprite> sprite, EntityType entityType, float xPos, float yPos)
{
	EntityId ammoEntityId = CreateEntityId();
	constexpr float zPos = 0.0f;
	constexpr float scale = 0.5f;

	auto tag = make_shared<Tag>(entityType);
	auto transform = make_shared<Transform>(glm::vec3(xPos, yPos, zPos), glm::vec3(0.0f), glm::vec3(scale));
	auto renderable = make_shared<Renderable>(sprite);

	AddComponent(ammoEntityId, tag);
	AddComponent(ammoEntityId, transform);
	AddComponent(ammoEntityId, renderable);

	return ammoEntityId;
}

EntityId EntityManager::CreateHealthBarEntity(shared_ptr<CSimpleSprite> sprite, float xPos, float yPos)
{
	EntityId healthBarEntityId = CreateEntityId();
	constexpr float zPos = 0.0f;

	auto tag = make_shared<Tag>(EntityType::HEALTH_BAR);
	auto transform = make_shared<Transform>(glm::vec3(xPos, yPos, zPos), glm::vec3(0.0f), glm::vec3(1.0f));
	auto renderable = make_shared<Renderable>(sprite);
	auto animation = make_shared<Animation>();

	AddComponent(healthBarEntityId, tag);
	AddComponent(healthBarEntityId, transform);
	AddComponent(healthBarEntityId, renderable);
	AddComponent(healthBarEntityId, animation);

	return healthBarEntityId;
}

EntityId EntityManager::CreateScoreEntity()
{
	EntityId scoreEntityId = CreateEntityId();
	constexpr float yOffset = 50.0f;
	constexpr float xPos = ScreenHandler::SCREEN_WIDTH / 2;
	constexpr float yPos = ScreenHandler::SCREEN_HEIGHT - yOffset;
	constexpr float zPos = 0.0f;

	auto tag = make_shared<Tag>(EntityType::SCORE);
	auto transform = make_shared<Transform>(glm::vec3(xPos, yPos, zPos), glm::vec3(0.0f), glm::vec3(1.0f));
	auto score = make_shared<Score>();

	AddComponent(scoreEntityId, tag);
	AddComponent(scoreEntityId, transform);
	AddComponent(scoreEntityId, score);

	return scoreEntityId;
}

EntityId EntityManager::CreateTimerEntity()
{
	EntityId timerEntityId = CreateEntityId();
	constexpr float xOffset = 1000.0f;
	constexpr float yOffset = 50.0f;
	float xPos = ScreenHandler::SCREEN_WIDTH - xOffset;
	float yPos = ScreenHandler::SCREEN_HEIGHT - yOffset;
	constexpr float zPos = 0.0f;

	auto tag = make_shared<Tag>(EntityType::TIMER);
	auto transform = make_shared<Transform>(glm::vec3(xPos, yPos, zPos), glm::vec3(0.0f), glm::vec3(1.0f));
	auto timer = make_shared<Timer>();

	AddComponent(timerEntityId, tag);
	AddComponent(timerEntityId, transform);
	AddComponent(timerEntityId, timer);

	return timerEntityId;
}

void EntityManager::HideAmmoFilledEntity(int index)
{
	if (index >= 0 && index < m_ammoFilledEntities.size()) {
		auto ammoFilledSprite = GetComponent<Renderable>(m_ammoFilledEntities[index])->GetSprite();

		if (!ammoFilledSprite)
			return;

		ammoFilledSprite->SetVisible(false);
	}
}

void EntityManager::ShowAllAmmoFilledEntity()
{
	for (int i = 0; i < m_ammoFilledEntities.size(); i++)
	{
		auto ammoFilledSprite = GetComponent<Renderable>(m_ammoFilledEntities[i])->GetSprite();
		ammoFilledSprite->SetVisible(true);
	}
}

void EntityManager::MoveEntityToRandomPos(EntityId entityId)
{
	float xPos = Helper::GenerateFloat(ScreenHandler::SCREEN_LEFT, ScreenHandler::SCREEN_RIGHT);
	float yPos = Helper::GenerateFloat(ScreenHandler::SCREEN_TOP, ScreenHandler::SCREEN_BOTTOM);
	constexpr float zPos = 0.0f;

	glm::vec3 newPos = glm::vec3(xPos, yPos, zPos);
	auto transform = GetComponent<Transform>(entityId);
	transform->SetPosition(newPos);
}

void EntityManager::MarkEntityForDeletion(EntityId entityId)
{
	// Check if already marked for deletion
	if (find(m_entitiesToDelete.begin(), m_entitiesToDelete.end(), entityId) == m_entitiesToDelete.end()) {
		m_entitiesToDelete.push_back(entityId);
	}
}

void EntityManager::ProcessDeletions() {
	for (EntityId entityId : m_entitiesToDelete) {
		auto tag = GetComponent<Tag>(entityId);
		if (tag->GetEntityType() == EntityType::ENEMY) {
			auto enemyAnimation = GetComponent<Animation>(entityId);
			if (enemyAnimation && enemyAnimation->GetCooldownTimer() > 0.0f)
				continue;
			m_entityComponents.erase(entityId);
		}
		else
			m_entityComponents.erase(entityId);
	}
	m_entitiesToDelete.clear();
}

void EntityManager::ProcessBulletHitEnemy(Event event, float deltaTime, const glm::vec3& playerPos, float screenWidth, float screenHeight)
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
	EntityId playerEntityId, enemyEntityId;
	EntityId entity1Id = event.entities[0];
	EntityId entity2Id = event.entities[1];

	auto tag1 = entityManager.GetComponent<Tag>(entity1Id);
	auto tag2 = entityManager.GetComponent<Tag>(entity2Id);

	if (!tag1 || !tag2)
		return;

	if (tag1->GetEntityType() == EntityType::PLAYER) {
		playerEntityId = entity1Id;
		enemyEntityId = entity2Id;
	}
	else {
		playerEntityId = entity2Id;
		enemyEntityId = entity1Id;
	}

	MarkEntityForDeletion(enemyEntityId);
}
