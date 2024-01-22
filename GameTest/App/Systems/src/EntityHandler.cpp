#include "stdafx.h"
#include "Components/include/Renderable.h"
#include "Components/include/Screen.h"
#include "Components/include/Transform.h"
#include "Components/include/Velocity.h"
#include "Managers/include/EntityManager.h"
#include "Managers/include/GameManager.h"
#include "Systems/include/EntityHandler.h"
#include "ParticleSystem/include/ParticleManager.h"
#include "Utilities/include/Helper.h"
#include <algorithm>
#include <random>
using glm::vec3;
using std::min;
using std::default_random_engine;

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
		{
			SetEnemiesToStrike(entityManager);
			InitLightningStrikes(entityManager);
			EmitSteamParticles();
		}
	}
}

void EntityHandler::InitEnemy()
{
	EntityManager& entityManager = EntityManager::GetInstance();
	SpriteManager& spriteManager = SpriteManager::GetInstance();
	Screen& screen = Screen::GetInstance();
	EntityId enemyEntityId = entityManager.GetEnemyFromPool();
	EntityId playerEntityId = entityManager.GetPlayerEntityId();

	const float borderWidth = (screen.BORDER_RIGHT_SCREEN_COORD - screen.BORDER_LEFT_SCREEN_COORD);
	const float borderHeight = (screen.BORDER_BOTTOM_SCREEN_COORD - screen.BORDER_TOP_SCREEN_COORD);

	vec3 playerPos = entityManager.GetComponent<Transform>(playerEntityId)->GetPosition();
	vec3 enemyPos = Helper::GetOppositeQuadrantPosition(playerPos, borderWidth, borderHeight);
	Transform* enemyTransform = entityManager.GetComponent<Transform>(enemyEntityId);
	Velocity* enemyVelocity = entityManager.GetComponent<Velocity>(enemyEntityId);
	CSimpleSprite* enemySprite = entityManager.GetComponent<Renderable>(enemyEntityId)->GetSprite();
	vec2 randomVelocity = Helper::GenerateVec2(MIN_VX, MAX_VX, MIN_VY, MAX_VY);

	enemyTransform->SetPosition(enemyPos);
	enemySprite->SetIsVisible(true);
	enemyVelocity->SetVelocity(randomVelocity);
	enemySprite->SetAnimation(ENEMY_ANIM_IDLE);

	GameManager::GetInstance().SetIsFirstEnemyInit(true);
}

void EntityHandler::MoveEntityToRandomPos(EntityId entityId)
{
	Screen& screen = screen.GetInstance();

	const float xPos = Helper::GenerateFloat(screen.BORDER_LEFT_SCREEN_COORD + OFFSET, screen.BORDER_RIGHT_SCREEN_COORD - OFFSET);
	const float yPos = Helper::GenerateFloat(screen.BORDER_TOP_SCREEN_COORD + OFFSET, screen.BORDER_BOTTOM_SCREEN_COORD - OFFSET);

	vec3 newPos = vec3(xPos, yPos, ZERO_POS_FLOAT);
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

void EntityHandler::SetEnemiesToStrike(EntityManager& entityManager)
{
	random_device rd;
	default_random_engine rng(rd());
	SpriteManager& spriteManager = SpriteManager::GetInstance();
	vector<EntityId> allEntityIds = entityManager.GetAllEntityIds();

	for (EntityId entityId : allEntityIds)
	{
		Tag* tag = entityManager.GetComponent<Tag>(entityId);
		if (tag->GetEntityType() == EntityType::Enemy && tag->GetEntityState() == EntityState::Alive)
			m_activeEnemiesEntityIds.push_back(entityId);
	}

	shuffle(m_activeEnemiesEntityIds.begin(), m_activeEnemiesEntityIds.end(), rng);
	m_numOfEnemiesToStrike = min(static_cast<int>(m_activeEnemiesEntityIds.size()), MAX_ENEMIES_TO_STRIKE);
}

void EntityHandler::InitLightningStrikes(EntityManager& entityManager)
{
	for (int i = 0; i < m_numOfEnemiesToStrike; ++i)
	{
		EntityId enemyEntityId = m_activeEnemiesEntityIds[i];
		Tag* enemyTag = entityManager.GetComponent<Tag>(enemyEntityId);
		Transform* enemyTransform = entityManager.GetComponent<Transform>(enemyEntityId);
		Velocity* enemyVelocity = entityManager.GetComponent<Velocity>(enemyEntityId);
		EntityId lightningStrikeEntityId = entityManager.GetLightningStrikeFromPool();
		Tag* lightningStrikeTag = entityManager.GetComponent<Tag>(lightningStrikeEntityId);
		Transform* lightningStrikeTransform = entityManager.GetComponent<Transform>(lightningStrikeEntityId);

		enemyVelocity->SetVelocity(ZERO_VELOCITY);
		enemyTag->SetEntityState(EntityState::HitByBullet);
		lightningStrikeTransform->SetPosition(enemyTransform->GetPosition());
	}
}

 void EntityHandler::EmitSteamParticles()
 {
	EntityManager& entityManager = EntityManager::GetInstance();
	ParticleManager& particleManager = ParticleManager::GetInstance();
	
 	const float minVelocityX = -10.0f;
 	const float maxVelocityX = 10.0f;
 	const float minVelocityY = 20.0f;
 	const float maxVelocityY = 50.0f;

 	 for (int i = 0; i < m_numOfEnemiesToStrike; ++i)
 	 {
 	 	EntityId enemyEntityId = m_activeEnemiesEntityIds[i];
	 	Transform* enemyTransform = entityManager.GetComponent<Transform>(enemyEntityId);
	 	vec3 emissionPos = enemyTransform->GetPosition();

 	 	for (int j = 0; j < particleManager.STEAM_PARTICLES_PER_ENEMY; ++j)
 	 	{
 	 		vec2 variedVelocityVec2 = Helper::GenerateVec2(minVelocityX, maxVelocityX, minVelocityY, maxVelocityY);
	 		vec3 variedVelocityVec3 = vec3(variedVelocityVec2, 0.0f);
 	 		particleManager.EmitParticles(ParticleType::Steam, emissionPos, variedVelocityVec3);
 	 	}
 	 }

 	m_activeEnemiesEntityIds.clear();
 }
