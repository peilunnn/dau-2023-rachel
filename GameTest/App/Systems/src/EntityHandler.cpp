#include "stdafx.h"
#include "Components/include/Animation.h"
#include "Components/include/Renderable.h"
#include "Components/include/Screen.h"
#include "Components/include/Transform.h"
#include "Components/include/Velocity.h"
#include "Managers/include/EntityManager.h"
#include "Managers/include/GameManager.h"
#include "Systems/include/AnimationHandler.h"
#include "Systems/include/EntityHandler.h"
#include "Utilities/include/Helper.h"
#include <algorithm>
#include <random>
using namespace std;
using glm::vec3;

EntityHandler& EntityHandler::GetInstance()
{
	static EntityHandler instance;
	return instance;
}

void EntityHandler::HandleEvent(const Event &event, float deltaTime)
{
	EntityManager &entityManager = EntityManager::GetInstance();

	if (event.GetEventType() == EventType::BulletHitEnemy)
	{
		EntityId bulletEntityId = event.GetEntities()[0];
		EntityId enemyEntityId = event.GetEntities()[1];
		HandleBulletHitEnemy(entityManager, bulletEntityId, enemyEntityId);
	}
	else if (event.GetEventType() == EventType::EnemyHitPlayer)
	{
		EntityId enemyEntityId = event.GetEntities()[0];
		HandleEnemyHitPlayer(entityManager, enemyEntityId);
	}
	else if (event.GetEventType() == EventType::BulletOutOfBounds)
	{
		EntityId bulletEntityId = event.GetEntities()[0];
		HandleBulletOutOfBounds(entityManager, bulletEntityId);
	}
	else if (event.GetEventType() == EventType::PlayerHitAmmoPickup || event.GetEventType() == EventType::PlayerHitHealthPickup || event.GetEventType() == EventType::PlayerHitLightningPickup)
	{
		EntityId pickupEntityId = event.GetEntities()[1];
		MoveEntityToRandomPos(pickupEntityId);

		if (event.GetEventType() == EventType::PlayerHitLightningPickup)
			InitLightningStrikes(entityManager);
	}
}

void EntityHandler::InitEnemy()
{
	EntityManager& entityManager = EntityManager::GetInstance();
	EntityId enemyEntityId = entityManager.GetEnemyFromPool();

	SpriteManager& spriteManager = SpriteManager::GetInstance();
	Screen& screen = Screen::GetInstance();

	EntityId playerEntityId = entityManager.GetPlayerEntityId();
	vec3 playerPos = entityManager.GetComponent<Transform>(playerEntityId)->GetPosition();
	const float borderWidth = (screen.BORDER_RIGHT_SCREEN_COORD - screen.BORDER_LEFT_SCREEN_COORD);
	const float borderHeight = (screen.BORDER_BOTTOM_SCREEN_COORD - screen.BORDER_TOP_SCREEN_COORD);
	vec3 enemyPos = Helper::GetOppositeQuadrantPosition(playerPos, borderWidth, borderHeight);
	Transform* enemyTransform = entityManager.GetComponent<Transform>(enemyEntityId);
	enemyTransform->SetPosition(enemyPos);

	CSimpleSprite* enemySprite = entityManager.GetComponent<Renderable>(enemyEntityId)->GetSprite();
	enemySprite->SetIsVisible(true);

	constexpr float minVx = -100.0f;
	constexpr float maxVx = 300.0f;
	constexpr float minVy = -100.0f;
	constexpr float maxVy = 300.0f;
	vec2 randomVelocity = Helper::GenerateVec2(minVx, maxVx, minVy, maxVy);
	Velocity* enemyVelocity = entityManager.GetComponent<Velocity>(enemyEntityId);
	enemyVelocity->SetVelocity(randomVelocity);

	Animation* enemyAnimation = entityManager.GetComponent<Animation>(enemyEntityId);
	enemyAnimation->SetCurrentAnimation(ENEMY_ANIM_IDLE);
	enemySprite->SetAnimation(enemyAnimation->GetCurrentAnimation());

	GameManager::GetInstance().SetIsFirstEnemyInit(true);
}

void EntityHandler::MoveEntityToRandomPos(EntityId entityId)
{
	Screen& screen = screen.GetInstance();

	constexpr float offset = 20.0f;
	const float xPos = Helper::GenerateFloat(screen.BORDER_LEFT_SCREEN_COORD + offset, screen.BORDER_RIGHT_SCREEN_COORD - offset);
	const float yPos = Helper::GenerateFloat(screen.BORDER_TOP_SCREEN_COORD + offset, screen.BORDER_BOTTOM_SCREEN_COORD - offset);
	constexpr float zPos = 0.0f;

	vec3 newPos = vec3(xPos, yPos, zPos);
	Transform* transform = EntityManager::GetInstance().GetComponent<Transform>(entityId);
	transform->SetPosition(newPos);
}

void EntityHandler::HandleBulletHitEnemy(EntityManager &entityManager, EntityId bulletEntityId, EntityId enemyEntityId)
{
	InitEnemy();
	InitEnemy();
	entityManager.ReturnBulletToPool(bulletEntityId);
}

void EntityHandler::HandleEnemyHitPlayer(EntityManager &entityManager, EntityId enemyEntityId)
{
	entityManager.ReturnEnemyToPool(enemyEntityId);
}

void EntityHandler::HandleBulletOutOfBounds(EntityManager &entityManager, EntityId bulletEntityId)
{
	entityManager.ReturnBulletToPool(bulletEntityId);
}

void EntityHandler::InitLightningStrikes(EntityManager& entityManager)
{
	SpriteManager& spriteManager = SpriteManager::GetInstance();
	vector<EntityId> allEntityIds = entityManager.GetAllEntityIds();

	vector<EntityId> activeEnemies;
	for (EntityId entityId : allEntityIds)
	{
		Tag* tag = entityManager.GetComponent<Tag>(entityId);
		if (tag && tag->GetEntityType() == EntityType::Enemy && tag->GetEntityState() == EntityState::Alive)
			activeEnemies.push_back(entityId);
	}

	random_device rd;
	default_random_engine rng(rd());

	// Randomly select up to 5 enemies
	shuffle(activeEnemies.begin(), activeEnemies.end(), rng);
	int enemiesToStrike = min(static_cast<int>(activeEnemies.size()), 5);

	for (int i = 0; i < enemiesToStrike; ++i)
	{
		EntityId enemyEntityId = activeEnemies[i];
		Tag* enemyTag = entityManager.GetComponent<Tag>(enemyEntityId);
		Transform* enemyTransform = entityManager.GetComponent<Transform>(enemyEntityId);
		Velocity* enemyVelocity = entityManager.GetComponent<Velocity>(enemyEntityId);
		constexpr vec2 zeroVelocity = vec2(0.0f);
		enemyVelocity->SetVelocity(zeroVelocity);
		enemyTag->SetEntityState(EntityState::HitByBullet);
		
		EntityId lightningStrikeEntityId = entityManager.GetLightningStrikeFromPool();
		Tag* lightningStrikeTag = entityManager.GetComponent<Tag>(lightningStrikeEntityId);
		Transform* lightningStrikeTransform = entityManager.GetComponent<Transform>(lightningStrikeEntityId);
		lightningStrikeTransform->SetPosition(enemyTransform->GetPosition());
	}
}
