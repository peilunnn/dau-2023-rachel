#include "stdafx.h"
#include "Components/include/Animation.h"
#include "Components/include/Collider.h"
#include "Components/include/BounceDirection.h"
#include "Components/include/EntityId.h"
#include "Components/include/Health.h"
#include "Components/include/Renderable.h"
#include "Components/include/Score.h"
#include "Components/include/Tag.h"
#include "Components/include/Timer.h"
#include "Components/include/Transform.h"
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

EntityId EntityManager::s_nextEntityId = 0;

void EntityManager::Init(shared_ptr<CSimpleSprite> playerSprite, shared_ptr<CSimpleSprite> enemySprite, shared_ptr<CSimpleSprite> reloadingCircleSprite, shared_ptr<CSimpleSprite> ammoEmptySprite, shared_ptr<CSimpleSprite> ammoFilledSprite, shared_ptr<CSimpleSprite> healthBarSprite)
{
	constexpr float screenWidth = ScreenHandler::SCREEN_WIDTH;
	constexpr float screenHeight = ScreenHandler::SCREEN_HEIGHT;
	constexpr float ammoSpriteSpacing = ScreenHandler::AMMO_SPRITE_SPACING;
	constexpr int maxBullets = ShootingHandler::MAX_BULLETS;
	constexpr float ammoXOffset = 20.0f;
	constexpr float ammoYOffset = 720.0f;
	constexpr float healthBarXOffset = 880.0f;
	constexpr float healthBarYOffset = 720.0f;
	constexpr float ammoStartingX = screenWidth - ammoXOffset;
	constexpr float ammoYPos = screenHeight - ammoYOffset;
	constexpr float healthBarXPos = screenWidth - healthBarXOffset;
	constexpr float healthBarYPos = screenHeight - healthBarYOffset;

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
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(0.75f);
	vec2 vel = vec2(0.0f);
	constexpr float dimensionsMultiplier = 1.0f;
	constexpr float radiusMultiplier = 0.5f;
	SpriteDimensions dimensions = Helper::GetSpriteDimensions(playerSprite, dimensionsMultiplier);

	shared_ptr<Tag> tag = make_shared<Tag>(EntityType::Player);
	shared_ptr<Transform> transform = make_shared<Transform>(vec3(xPos, yPos, zPos), rot, scale);
	shared_ptr<Renderable> renderable = make_shared<Renderable>(playerSprite);
	shared_ptr<Collider> collider = make_shared<Collider>();
	collider->SetCollisionShape(CollisionShape::Sphere);
	collider->SetCollisionType(CollisionType::Player);
	collider->SetCollisionMask(static_cast<int>(CollisionType::Enemy) | static_cast<int>(CollisionType::ReloadingCircle));
	collider->SetRadius(dimensions.width * radiusMultiplier);
	shared_ptr<Health> health = make_shared<Health>();
	shared_ptr<Animation> animation = make_shared<Animation>();

	EntityManager::AddComponent(playerEntityId, tag);
	EntityManager::AddComponent(playerEntityId, transform);
	EntityManager::AddComponent(playerEntityId, renderable);
	EntityManager::AddComponent(playerEntityId, collider);
	EntityManager::AddComponent(playerEntityId, health);
	EntityManager::AddComponent(playerEntityId, animation);

	return playerEntityId;
}

EntityId EntityManager::CreateEnemyEntity(const vec3 &playerPos, shared_ptr<CSimpleSprite> enemySprite, float screenWidth, float screenHeight)
{
	EntityId enemyEntityId = CreateEntityId();
	constexpr float minVx = -100.0f;
	constexpr float maxVx = 300.0f;
	constexpr float minVy = -100.0;
	constexpr float maxVy = 300.0f;
	vec3 pos = Helper::GetOppositeQuadrantPosition(playerPos, ScreenHandler::SCREEN_WIDTH, ScreenHandler::SCREEN_HEIGHT);
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(0.4f);
	constexpr float dimensionsMultiplier = 1.0f;
	constexpr float radiusMultiplier = 0.5f;
	SpriteDimensions dimensions = Helper::GetSpriteDimensions(enemySprite, dimensionsMultiplier);

	shared_ptr<Tag> tag = make_shared<Tag>(EntityType::Enemy);
	shared_ptr<Transform> transform = make_shared<Transform>(pos, rot, scale);
	shared_ptr<Renderable> renderable = make_shared<Renderable>(enemySprite);
	shared_ptr<Collider> collider = make_shared<Collider>();
	collider->SetCollisionShape(CollisionShape::Sphere);
	collider->SetCollisionType(CollisionType::Enemy);
	collider->SetCollisionMask(static_cast<int>(CollisionType::Player) | static_cast<int>(CollisionType::Bullet));
	collider->SetRadius(dimensions.width * radiusMultiplier);
	shared_ptr<BounceDirection> bounceDirection = make_shared<BounceDirection>();
	shared_ptr<Animation> animation = make_shared<Animation>();
	vec2 randomVelocity = Helper::GenerateVec2(minVx, maxVx, minVy, maxVy);
	MovementHandler::SetVelocity(enemyEntityId, randomVelocity);

	EntityManager::AddComponent(enemyEntityId, tag);
	EntityManager::AddComponent(enemyEntityId, transform);
	EntityManager::AddComponent(enemyEntityId, renderable);
	EntityManager::AddComponent(enemyEntityId, collider);
	EntityManager::AddComponent(enemyEntityId, bounceDirection);
	EntityManager::AddComponent(enemyEntityId, animation);

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

	shared_ptr<Tag> tag = make_shared<Tag>(EntityType::Bullet);
	shared_ptr<Transform> transform = make_shared<Transform>(pos, rot, scale);
	shared_ptr<Renderable> renderable = make_shared<Renderable>(bulletSprite);
	shared_ptr<Collider> collider = make_shared<Collider>();
	collider->SetCollisionShape(CollisionShape::Sphere);
	collider->SetCollisionType(CollisionType::Bullet);
	collider->SetCollisionMask(static_cast<int>(CollisionType::Enemy));
	collider->SetRadius(dimensions.width * radiusMultiplier);
	MovementHandler::SetVelocity(bulletEntityId, targetVelocity);

	AddComponent(bulletEntityId, tag);
	AddComponent(bulletEntityId, transform);
	AddComponent(bulletEntityId, renderable);
	AddComponent(bulletEntityId, collider);

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
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(0.4f);
	constexpr float dimensionsMultiplier = 1.0f;
	constexpr float radiusMultiplier = 0.5f;
	SpriteDimensions dimensions = Helper::GetSpriteDimensions(reloadingCircleSprite, dimensionsMultiplier);

	shared_ptr<Tag> tag = make_shared<Tag>(EntityType::ReloadingCircle);
	shared_ptr<Transform> transform = make_shared<Transform>(vec3(xPos, yPos, zPos), rot, scale);
	shared_ptr<Renderable> renderable = make_shared<Renderable>(reloadingCircleSprite);
	shared_ptr<Collider> collider = make_shared<Collider>();
	collider->SetCollisionShape(CollisionShape::Sphere);
	collider->SetCollisionType(CollisionType::ReloadingCircle);
	collider->SetCollisionMask(static_cast<int>(CollisionType::Player));
	collider->SetRadius(dimensions.width * radiusMultiplier);
	shared_ptr<Animation> animation = make_shared<Animation>();

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
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(0.5f);

	shared_ptr<Tag> tag = make_shared<Tag>(entityType);
	shared_ptr<Transform> transform = make_shared<Transform>(vec3(xPos, yPos, zPos), vec3(0.0f), vec3(scale));
	shared_ptr<Renderable> renderable = make_shared<Renderable>(sprite);

	AddComponent(ammoEntityId, tag);
	AddComponent(ammoEntityId, transform);
	AddComponent(ammoEntityId, renderable);

	return ammoEntityId;
}

EntityId EntityManager::CreateHealthBarEntity(shared_ptr<CSimpleSprite> sprite, float xPos, float yPos)
{
	EntityId healthBarEntityId = CreateEntityId();
	constexpr float zPos = 0.0f;
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(1.0f);

	shared_ptr<Tag> tag = make_shared<Tag>(EntityType::HealthBar);
	shared_ptr<Transform> transform = make_shared<Transform>(vec3(xPos, yPos, zPos), rot, scale);
	shared_ptr<Renderable> renderable = make_shared<Renderable>(sprite);
	shared_ptr<Animation> animation = make_shared<Animation>();

	AddComponent(healthBarEntityId, tag);
	AddComponent(healthBarEntityId, transform);
	AddComponent(healthBarEntityId, renderable);
	AddComponent(healthBarEntityId, animation);

	return healthBarEntityId;
}

EntityId EntityManager::CreateScoreEntity()
{
	EntityId scoreEntityId = CreateEntityId();
	constexpr float xOffset = 100.0f;
	constexpr float yOffset = 50.0f;
	constexpr float xPos = ScreenHandler::SCREEN_WIDTH - xOffset;
	constexpr float yPos = ScreenHandler::SCREEN_HEIGHT - yOffset;
	constexpr float zPos = 0.0f;
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(1.0f);

	shared_ptr<Tag> tag = make_shared<Tag>(EntityType::Score);
	shared_ptr<Transform> transform = make_shared<Transform>(vec3(xPos, yPos, zPos), rot, scale);
	shared_ptr<Score> score = make_shared<Score>();

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
	constexpr vec3 rot = vec3(0.0f);
	constexpr vec3 scale = vec3(1.0f);

	shared_ptr<Tag> tag = make_shared<Tag>(EntityType::Timer);
	shared_ptr<Transform> transform = make_shared<Transform>(vec3(xPos, yPos, zPos), rot, scale);
	shared_ptr<Timer> timer = make_shared<Timer>();

	AddComponent(timerEntityId, tag);
	AddComponent(timerEntityId, transform);
	AddComponent(timerEntityId, timer);

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
	float xPos = Helper::GenerateFloat(ScreenHandler::SCREEN_LEFT, ScreenHandler::SCREEN_RIGHT);
	float yPos = Helper::GenerateFloat(ScreenHandler::SCREEN_TOP, ScreenHandler::SCREEN_BOTTOM);
	constexpr float zPos = 0.0f;

	vec3 newPos = vec3(xPos, yPos, zPos);
	shared_ptr<Transform> transform = GetComponent<Transform>(entityId);
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
		shared_ptr<Tag> tag = GetComponent<Tag>(entityId);
		if (tag->GetEntityType() == EntityType::Enemy)
		{
			shared_ptr<Animation> enemyAnimation = GetComponent<Animation>(entityId);
			if (enemyAnimation && enemyAnimation->GetCooldownTimer() > 0.0f)
				continue;
			m_entityComponents.erase(entityId);
		}
		else
		{
			m_entityComponents.erase(entityId);
		}
	}
	m_entitiesToDelete.clear();
}