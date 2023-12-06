#include "stdafx.h"
#include "Components/include/Animation.h"
#include "Components/include/BounceDirection.h"
#include "Components/include/Collider.h"
#include "Components/include/Cooldown.h"
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
#include "Systems/include/MovementHandler.h"
#include "Utilities/include/App.h"
#include "Utilities/include/Helper.h"
using glm::vec2;
using glm::vec3;

void EntityManager::Init(shared_ptr<CSimpleSprite> playerSprite, shared_ptr<CSimpleSprite> enemySprite, shared_ptr<CSimpleSprite> reloadingCircleSprite, shared_ptr<CSimpleSprite> ammoEmptySprite, shared_ptr<CSimpleSprite> ammoFilledSprite, shared_ptr<CSimpleSprite> healthBarSprite)
{
	ScreenHandler& screenHandler = screenHandler.GetInstance();
	ShootingHandler& shootingHandler = ShootingHandler::GetInstance();

	const float screenWidth = screenHandler.SCREEN_WIDTH;
	const float screenHeight = screenHandler.SCREEN_HEIGHT;
	const int ammoSpriteSpacing = screenHandler.AMMO_SPRITE_SPACING;
	const float ammoXOffset = 20.0f;
	const float ammoYOffset = 720.0f;
	const float healthBarXOffset = 880.0f;
	const float healthBarYOffset = 720.0f;
	const float ammoStartingX = screenWidth - ammoXOffset;
	const float ammoYPos = screenHeight - ammoYOffset;
	const float healthBarXPos = screenWidth - healthBarXOffset;
	const float healthBarYPos = screenHeight - healthBarYOffset;
	const int maxBullets = shootingHandler.MAX_BULLETS;

	m_playerEntityId = CreatePlayerEntity(playerSprite);
	vec3 playerPos = GetComponent<Transform>(m_playerEntityId)->GetPosition();
	m_enemyEntityId = CreateEnemyEntity(playerPos, enemySprite, screenWidth, screenHeight);
	m_reloadingCircleEntityId = CreateReloadingCircleEntity(reloadingCircleSprite);
	m_healthBarEntityId = CreateHealthBarEntity(healthBarSprite, healthBarXPos, healthBarYPos);

	for (int i = 0; i < maxBullets; ++i)
	{
		float xPos = ammoStartingX - i * ammoSpriteSpacing;
		constexpr int columns = 1;
		constexpr int rows = 1;

		// Have to create a new sprite for every entity
		CSimpleSprite *rawAmmoEmptySprite = App::CreateSprite(Helper::PATH_TO_AMMO_EMPTY_SPRITE, columns, rows);
		shared_ptr<CSimpleSprite> ammoEmptySprite = shared_ptr<CSimpleSprite>(rawAmmoEmptySprite);
		CSimpleSprite *rawAmmoFilledSprite = App::CreateSprite(Helper::PATH_TO_AMMO_FILLED_SPRITE, columns, rows);
		shared_ptr<CSimpleSprite> ammoFilledSprite = shared_ptr<CSimpleSprite>(rawAmmoFilledSprite);

		m_ammoEmptyEntityId = CreateAmmoEntity(ammoEmptySprite, EntityType::AmmoEmpty, xPos, ammoYPos);
		m_ammoEmptyEntities.push_back(m_ammoEmptyEntityId);
		m_ammoFilledEntityId = CreateAmmoEntity(ammoFilledSprite, EntityType::AmmoFilled, xPos, ammoYPos);
		m_ammoFilledEntities.push_back(m_ammoFilledEntityId);
	}

	m_scoreEntityId = CreateScoreEntity();
	m_timerEntityId = CreateTimerEntity();
}

vector<EntityId> EntityManager::GetAllEntities()
{
	vector<EntityId> allEntities;
	for (const auto &pair : m_entityComponents)
	{
		allEntities.push_back(pair.first);
	}
	return allEntities;
}

EntityId EntityManager::CreateEntityId()
{
	return m_nextEntityId++;
}

EntityId EntityManager::CreatePlayerEntity(shared_ptr<CSimpleSprite> playerSprite)
{
	EntityId playerEntityId = CreateEntityId();
	ScreenHandler& screenHandler = screenHandler.GetInstance();
	const float maxX = screenHandler.SCREEN_RIGHT - screenHandler.SCREEN_LEFT;
	const float maxY = screenHandler.SCREEN_BOTTOM - screenHandler.SCREEN_TOP;
	float xPos = Helper::GenerateFloat(screenHandler.SCREEN_LEFT, screenHandler.SCREEN_RIGHT);
	float yPos = Helper::GenerateFloat(screenHandler.SCREEN_TOP, screenHandler.SCREEN_BOTTOM);
	constexpr float zPos = 0.0f;
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(0.75f);
	vec2 vel = vec2(0.0f);
	constexpr float dimensionsMultiplier = 1.0f;
	constexpr float radiusMultiplier = 0.5f;
	SpriteDimensions dimensions = Helper::GetSpriteDimensions(playerSprite, dimensionsMultiplier);
	constexpr float playerShootingCooldown = 1.0f;

	unique_ptr<Tag> tag = make_unique<Tag>(EntityType::Player);
	unique_ptr<Transform> transform = make_unique<Transform>(vec3(xPos, yPos, zPos), rot, scale);
	unique_ptr<Renderable> renderable = make_unique<Renderable>(playerSprite);
	unique_ptr<Collider> collider = make_unique<Collider>();
	collider->SetCollisionShape(CollisionShape::Sphere);
	collider->SetCollisionType(CollisionType::Player);
	collider->SetCollisionMask(static_cast<int>(CollisionType::Enemy) | static_cast<int>(CollisionType::ReloadingCircle));
	collider->SetRadius(dimensions.width * radiusMultiplier);
	unique_ptr<Velocity> velocity = make_unique<Velocity>();
	unique_ptr<Health> health = make_unique<Health>();
	unique_ptr<Animation> animation = make_unique<Animation>();
	unique_ptr<Cooldown> cooldown = make_unique<Cooldown>(playerShootingCooldown);

	AddComponent(playerEntityId, move(tag));
	AddComponent(playerEntityId, move(transform));
	AddComponent(playerEntityId, move(renderable));
	AddComponent(playerEntityId, move(collider));
	AddComponent(playerEntityId, move(velocity));
	AddComponent(playerEntityId, move(health));
	AddComponent(playerEntityId, move(animation));
	AddComponent(playerEntityId, move(cooldown));

	return playerEntityId;
}

EntityId EntityManager::CreateEnemyEntity(const vec3 &playerPos, shared_ptr<CSimpleSprite> enemySprite, float screenWidth, float screenHeight)
{
	EntityId enemyEntityId = CreateEntityId();
	ScreenHandler& screenHandler = screenHandler.GetInstance();
	constexpr float minVx = -100.0f;
	constexpr float maxVx = 300.0f;
	constexpr float minVy = -100.0;
	constexpr float maxVy = 300.0f;
	vec3 pos = Helper::GetOppositeQuadrantPosition(playerPos, screenHandler.SCREEN_WIDTH, screenHandler.SCREEN_HEIGHT);
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(0.4f);
	constexpr float dimensionsMultiplier = 1.0f;
	constexpr float radiusMultiplier = 0.5f;
	SpriteDimensions dimensions = Helper::GetSpriteDimensions(enemySprite, dimensionsMultiplier);
	vec2 randomVelocity = Helper::GenerateVec2(minVx, maxVx, minVy, maxVy);

	unique_ptr<Tag> tag = make_unique<Tag>(EntityType::Enemy);
	unique_ptr<Transform> transform = make_unique<Transform>(pos, rot, scale);
	unique_ptr<Renderable> renderable = make_unique<Renderable>(enemySprite);
	unique_ptr<Collider> collider = make_unique<Collider>();
	collider->SetCollisionShape(CollisionShape::Sphere);
	collider->SetCollisionType(CollisionType::Enemy);
	collider->SetCollisionMask(static_cast<int>(CollisionType::Player) | static_cast<int>(CollisionType::Bullet));
	collider->SetRadius(dimensions.width * radiusMultiplier);
	unique_ptr<Velocity> velocity = make_unique<Velocity>(randomVelocity);
	unique_ptr<BounceDirection> bounceDirection = make_unique<BounceDirection>();
	unique_ptr<Animation> animation = make_unique<Animation>();

	AddComponent(enemyEntityId, move(tag));
	AddComponent(enemyEntityId, move(transform));
	AddComponent(enemyEntityId, move(renderable));
	AddComponent(enemyEntityId, move(collider));
	AddComponent(enemyEntityId, move(velocity));
	AddComponent(enemyEntityId, move(bounceDirection));
	AddComponent(enemyEntityId, move(animation));

	return enemyEntityId;
}

EntityId EntityManager::CreateBulletEntity(shared_ptr<CSimpleSprite> bulletSprite, const vec3 &pos, const vec2 &targetVelocity)
{
	EntityId bulletEntityId = CreateEntityId();
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(1.0f);
	constexpr float dimensionsMultiplier = 1.0f;
	constexpr float radiusMultiplier = 0.5f;
	SpriteDimensions dimensions = Helper::GetSpriteDimensions(bulletSprite, dimensionsMultiplier);

	unique_ptr<Tag> tag = make_unique<Tag>(EntityType::Bullet);
	unique_ptr<Transform> transform = make_unique<Transform>(pos, rot, scale);
	unique_ptr<Renderable> renderable = make_unique<Renderable>(bulletSprite);
	unique_ptr<Collider> collider = make_unique<Collider>();
	collider->SetCollisionShape(CollisionShape::Sphere);
	collider->SetCollisionType(CollisionType::Bullet);
	collider->SetCollisionMask(static_cast<int>(CollisionType::Enemy));
	collider->SetRadius(dimensions.width * radiusMultiplier);
	unique_ptr<Velocity> velocity = make_unique<Velocity>(targetVelocity);

	AddComponent(bulletEntityId, move(tag));
	AddComponent(bulletEntityId, move(transform));
	AddComponent(bulletEntityId, move(renderable));
	AddComponent(bulletEntityId, move(collider));
	AddComponent(bulletEntityId, move(velocity));

	return bulletEntityId;
}

EntityId EntityManager::CreateReloadingCircleEntity(shared_ptr<CSimpleSprite> reloadingCircleSprite)
{
	EntityId reloadingCircleEntityId = CreateEntityId();
	ScreenHandler& screenHandler = screenHandler.GetInstance();
	const float maxX = screenHandler.SCREEN_RIGHT - screenHandler.SCREEN_LEFT;
	const float maxY = screenHandler.SCREEN_BOTTOM - screenHandler.SCREEN_TOP;
	float xPos = Helper::GenerateFloat(screenHandler.SCREEN_LEFT, screenHandler.SCREEN_RIGHT);
	float yPos = Helper::GenerateFloat(screenHandler.SCREEN_TOP, screenHandler.SCREEN_BOTTOM);
	constexpr float zPos = 0.0f;
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(0.4f);
	constexpr float dimensionsMultiplier = 1.0f;
	constexpr float radiusMultiplier = 0.5f;
	SpriteDimensions dimensions = Helper::GetSpriteDimensions(reloadingCircleSprite, dimensionsMultiplier);

	unique_ptr<Tag> tag = make_unique<Tag>(EntityType::ReloadingCircle);
	unique_ptr<Transform> transform = make_unique<Transform>(vec3(xPos, yPos, zPos), rot, scale);
	unique_ptr<Renderable> renderable = make_unique<Renderable>(reloadingCircleSprite);
	unique_ptr<Collider> collider = make_unique<Collider>();
	collider->SetCollisionShape(CollisionShape::Sphere);
	collider->SetCollisionType(CollisionType::ReloadingCircle);
	collider->SetCollisionMask(static_cast<int>(CollisionType::Player));
	collider->SetRadius(dimensions.width * radiusMultiplier);
	unique_ptr<Animation> animation = make_unique<Animation>();

	AddComponent(reloadingCircleEntityId, move(tag));
	AddComponent(reloadingCircleEntityId, move(transform));
	AddComponent(reloadingCircleEntityId, move(renderable));
	AddComponent(reloadingCircleEntityId, move(collider));
	AddComponent(reloadingCircleEntityId, move(animation));

	return reloadingCircleEntityId;
}

EntityId EntityManager::CreateAmmoEntity(shared_ptr<CSimpleSprite> sprite, EntityType entityType, float xPos, float yPos)
{
	EntityId ammoEntityId = CreateEntityId();
	constexpr float zPos = 0.0f;
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(0.5f);

	unique_ptr<Tag> tag = make_unique<Tag>(entityType);
	unique_ptr<Transform> transform = make_unique<Transform>(vec3(xPos, yPos, zPos), rot, scale);
	unique_ptr<Renderable> renderable = make_unique<Renderable>(sprite);

	AddComponent(ammoEntityId, move(tag));
	AddComponent(ammoEntityId, move(transform));
	AddComponent(ammoEntityId, move(renderable));

	return ammoEntityId;
}

EntityId EntityManager::CreateHealthBarEntity(shared_ptr<CSimpleSprite> sprite, float xPos, float yPos)
{
	EntityId healthBarEntityId = CreateEntityId();
	constexpr float zPos = 0.0f;
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(1.0f);

	unique_ptr<Tag> tag = make_unique<Tag>(EntityType::HealthBar);
	unique_ptr<Transform> transform = make_unique<Transform>(vec3(xPos, yPos, zPos), rot, scale);
	unique_ptr<Renderable> renderable = make_unique<Renderable>(sprite);
	unique_ptr<Animation> animation = make_unique<Animation>();

	AddComponent(healthBarEntityId, move(tag));
	AddComponent(healthBarEntityId, move(transform));
	AddComponent(healthBarEntityId, move(renderable));
	AddComponent(healthBarEntityId, move(animation));

	return healthBarEntityId;
}

EntityId EntityManager::CreateScoreEntity()
{
	EntityId scoreEntityId = CreateEntityId();
	ScreenHandler& screenHandler = screenHandler.GetInstance();
	constexpr float xOffset = 100.0f;
	constexpr float yOffset = 50.0f;
	const float xPos = screenHandler.SCREEN_WIDTH - xOffset;
	const float yPos = screenHandler.SCREEN_HEIGHT - yOffset;
	constexpr float zPos = 0.0f;
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(1.0f);

	unique_ptr<Tag> tag = make_unique<Tag>(EntityType::Score);
	unique_ptr<Transform> transform = make_unique<Transform>(vec3(xPos, yPos, zPos), rot, scale);
	unique_ptr<Score> score = make_unique<Score>();

	AddComponent(scoreEntityId, move(tag));
	AddComponent(scoreEntityId, move(transform));
	AddComponent(scoreEntityId, move(score));

	return scoreEntityId;
}

EntityId EntityManager::CreateTimerEntity()
{
	EntityId timerEntityId = CreateEntityId();
	ScreenHandler& screenHandler = screenHandler.GetInstance();
	constexpr float xOffset = 1000.0f;
	constexpr float yOffset = 50.0f;
	float xPos = screenHandler.SCREEN_WIDTH - xOffset;
	float yPos = screenHandler.SCREEN_HEIGHT - yOffset;
	constexpr float zPos = 0.0f;
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(1.0f);

	unique_ptr<Tag> tag = make_unique<Tag>(EntityType::Timer);
	unique_ptr<Transform> transform = make_unique<Transform>(vec3(xPos, yPos, zPos), rot, scale);
	unique_ptr<Timer> timer = make_unique<Timer>();

	AddComponent(timerEntityId, move(tag));
	AddComponent(timerEntityId, move(transform));
	AddComponent(timerEntityId, move(timer));

	return timerEntityId;
}

void EntityManager::HideAmmoFilledEntity(int index)
{
	if (index >= 0 && index < m_ammoFilledEntities.size())
	{
		shared_ptr<CSimpleSprite> ammoFilledSprite = GetComponent<Renderable>(m_ammoFilledEntities[index])->GetSprite();

		if (!ammoFilledSprite)
			return;

		ammoFilledSprite->SetVisible(false);
	}
}

void EntityManager::ShowAllAmmoFilledEntity()
{
	for (int i = 0; i < m_ammoFilledEntities.size(); i++)
	{
		shared_ptr<CSimpleSprite> ammoFilledSprite = GetComponent<Renderable>(m_ammoFilledEntities[i])->GetSprite();
		ammoFilledSprite->SetVisible(true);
	}
}

void EntityManager::MoveEntityToRandomPos(EntityId entityId)
{
	ScreenHandler& screenHandler = screenHandler.GetInstance();
	float xPos = Helper::GenerateFloat(screenHandler.SCREEN_LEFT, screenHandler.SCREEN_RIGHT);
	float yPos = Helper::GenerateFloat(screenHandler.SCREEN_TOP, screenHandler.SCREEN_BOTTOM);
	constexpr float zPos = 0.0f;

	vec3 newPos = vec3(xPos, yPos, zPos);
	Transform* transform = GetComponent<Transform>(entityId);
	transform->SetPosition(newPos);
}

void EntityManager::MarkEntityForDeletion(EntityId entityId)
{
	// Check if already marked for deletion
	if (find(m_entitiesToDelete.begin(), m_entitiesToDelete.end(), entityId) == m_entitiesToDelete.end())
	{
		m_entitiesToDelete.push_back(entityId);
	}
}

void EntityManager::ProcessDeletions()
{
	for (EntityId entityId : m_entitiesToDelete)
	{
		m_entityComponents.erase(entityId);
	}
	m_entitiesToDelete.clear();
}