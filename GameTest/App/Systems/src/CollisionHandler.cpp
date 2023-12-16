#include "stdafx.h"
#include "Components/include/Collider.h"
#include "Components/include/Tag.h"
#include "Managers/include/EntityManager.h"
#include "Managers/include/GameManager.h"
#include "Managers/include/SoundManager.h"
#include "Managers/include/SystemManager.h"
#include "Systems/include/AnimationHandler.h"
#include "Systems/include/CollisionHandler.h"
#include "Systems/include/Event.h"
#include "Systems/include/HealthHandler.h"
#include "Systems/include/EntityHandler.h"
#include "Systems/include/ShootingHandler.h"
#include "Systems/include/RenderingHandler.h"
#include "Utilities/include/Helper.h"
using glm::dot;
using glm::vec2;

map<EntityType, set<EntityType>> CollisionHandler::m_collisionRules =
	{
		{EntityType::Player, {EntityType::Enemy, EntityType::AmmoPickup, EntityType::HealthPickup, EntityType::LightningPickup}},
		{EntityType::Enemy, {EntityType::Player, EntityType::Bullet}},
};

CollisionHandler &CollisionHandler::GetInstance()
{
	static CollisionHandler instance;
	return instance;
}

void CollisionHandler::Update(float deltaTime)
{
	GameManager &gameManager = GameManager::GetInstance();
	EntityManager &entityManager = EntityManager::GetInstance();
	SystemManager &systemManager = SystemManager::GetInstance();
	vector<EntityId> allEntityIds = entityManager.GetAllEntityIds();

	if (gameManager.GetCurrentGameState() == GameState::Paused)
		return;

	for (EntityId &entityId : allEntityIds)
	{
		Collider *collider = entityManager.GetComponent<Collider>(entityId);
		if (collider)
			collider->SetCheckedForCollisions(false);
	}

	m_rootQuadtree.Clear();
	PopulateQuadtree(entityManager, allEntityIds);

	for (EntityId &entityId : allEntityIds)
	{
		Collider *collider = entityManager.GetComponent<Collider>(entityId);
		Transform *transform = entityManager.GetComponent<Transform>(entityId);
		EntityType entityType = entityManager.GetComponent<Tag>(entityId)->GetEntityType();

		if (!collider || !transform)
			continue;

		// Retrieve potential colliders' transforms and their associated entity id from the quadtree
		vector<QuadtreeElement> potentialElements = m_rootQuadtree.Retrieve(transform);
		for (QuadtreeElement &element : potentialElements)
		{
			// Ensure we are not comparing the entity with itself
			if (entityId == element.entityId)
				continue;

			EntityType potentialEntityType = entityManager.GetComponent<Tag>(element.entityId)->GetEntityType();

			// If the two entities are not even supposed to collide, we skip collision check
			if (m_collisionRules[entityType].find(potentialEntityType) == m_collisionRules[entityType].end())
				continue;

			Collider *potentialElementCollider = entityManager.GetComponent<Collider>(element.entityId);

			if (!potentialElementCollider)
				continue;

			// Check if the entities are colliding
			if (IsColliding(transform, collider, element.transform, potentialElementCollider))
			{
				HandleCollisionEvent(entityId, element.entityId);
				collider->SetCheckedForCollisions(true);
				potentialElementCollider->SetCheckedForCollisions(true);
			}
		}
	}
}

void CollisionHandler::PopulateQuadtree(EntityManager &entityManager, vector<EntityId> allEntityIds)
{
	for (EntityId &entityId : allEntityIds)
	{
		Transform *transform = entityManager.GetComponent<Transform>(entityId);
		if (transform)
			m_rootQuadtree.Insert(transform, entityId);
	}
}

bool CollisionHandler::IsColliding(Transform *firstTransform, Collider *firstCollider, Transform *secondTransform, Collider *secondCollider)
{
	if (!firstTransform || !secondTransform || !firstCollider || !secondCollider)
		return false;

	vec2 firstEntityPos = vec2(firstTransform->GetPosition().x, firstTransform->GetPosition().y);
	vec2 secondEntityPos = vec2(secondTransform->GetPosition().x, secondTransform->GetPosition().y);
	vec2 diff = firstEntityPos - secondEntityPos;
	float squaredDistance = dot(diff, diff);

	float totalRadius = firstCollider->GetRadius() + secondCollider->GetRadius();
	float squaredTotalRadius = totalRadius * totalRadius;

	return squaredDistance < squaredTotalRadius;
}

void CollisionHandler::HandleCollisionEvent(EntityId firstEntityId, EntityId secondEntityId)
{
	EntityManager &entityManager = EntityManager::GetInstance();
	SystemManager &systemManager = SystemManager::GetInstance();
	EntityType firstEntityType = entityManager.GetComponent<Tag>(firstEntityId)->GetEntityType();
	EntityType secondEntityType = entityManager.GetComponent<Tag>(secondEntityId)->GetEntityType();

	// Case 1: bullet-enemy
	if ((firstEntityType == EntityType::Bullet && secondEntityType == EntityType::Enemy) ||
		(firstEntityType == EntityType::Enemy && secondEntityType == EntityType::Bullet))
	{
		EntityId bulletEntityId = (firstEntityType == EntityType::Bullet) ? firstEntityId : secondEntityId;
		EntityId enemyEntityId = (firstEntityType == EntityType::Enemy) ? firstEntityId : secondEntityId;
		HandleBulletEnemyCollision(entityManager, systemManager, bulletEntityId, enemyEntityId);
	}

	// Case 2: player-enemy
	else if ((firstEntityType == EntityType::Player && secondEntityType == EntityType::Enemy) ||
			 (firstEntityType == EntityType::Enemy && secondEntityType == EntityType::Player))
	{
		EntityId playerEntityId = (firstEntityType == EntityType::Player) ? firstEntityId : secondEntityId;
		EntityId enemyEntityId = (firstEntityType == EntityType::Enemy) ? firstEntityId : secondEntityId;
		HandlePlayerEnemyCollision(entityManager, systemManager, playerEntityId, enemyEntityId);
	}

	// Case 3: player-ammoPickup
	else if ((firstEntityType == EntityType::Player && secondEntityType == EntityType::AmmoPickup) ||
			 (firstEntityType == EntityType::AmmoPickup && secondEntityType == EntityType::Player))
	{
		EntityId playerEntityId = (firstEntityType == EntityType::Player) ? firstEntityId : secondEntityId;
		EntityId ammoPickupEntityId = (firstEntityType == EntityType::AmmoPickup) ? firstEntityId : secondEntityId;
		HandlePlayerAmmoPickupCollision(entityManager, systemManager, playerEntityId, ammoPickupEntityId);
	}

	// Case 4: player-healthPickup
	else if ((firstEntityType == EntityType::Player && secondEntityType == EntityType::HealthPickup) ||
			 (firstEntityType == EntityType::HealthPickup && secondEntityType == EntityType::Player))
	{
		EntityId playerEntityId = (firstEntityType == EntityType::Player) ? firstEntityId : secondEntityId;
		EntityId healthPickupEntityId = (firstEntityType == EntityType::HealthPickup) ? firstEntityId : secondEntityId;
		HandlePlayerHealthPickupCollision(entityManager, systemManager, playerEntityId, healthPickupEntityId);
	}

	// Case 5: player-lightningPickup
	else if ((firstEntityType == EntityType::Player && secondEntityType == EntityType::LightningPickup) ||
			 (firstEntityType == EntityType::LightningPickup && secondEntityType == EntityType::Player))
	{
		EntityId playerEntityId = (firstEntityType == EntityType::Player) ? firstEntityId : secondEntityId;
		EntityId lightningPickupEntityId = (firstEntityType == EntityType::LightningPickup) ? firstEntityId : secondEntityId;
		HandlePlayerLightningPickupCollision(entityManager, systemManager, playerEntityId, lightningPickupEntityId);
	}
}

void CollisionHandler::HandleBulletEnemyCollision(EntityManager &entityManager, SystemManager &systemManager, EntityId bulletEntityId, EntityId enemyEntityId)
{
	Tag *bulletTag = entityManager.GetComponent<Tag>(bulletEntityId);
	Tag *enemyTag = entityManager.GetComponent<Tag>(enemyEntityId);

	if (bulletTag->GetEntityState() == EntityState::Dead)
		return;

	// If enemy is inactive (in pool) or was just hit in the same frame, we don't process the collision
	if (enemyTag->GetEntityState() == EntityState::Dead || enemyTag->GetEntityState() == EntityState::HitByBullet)
		return;

	bulletTag->SetEntityState(EntityState::Dead);
	enemyTag->SetEntityState(EntityState::HitByBullet);

	Event bulletHitEnemyEvent(EventType::BulletHitEnemy, {bulletEntityId, enemyEntityId});
	systemManager.SendEvent(bulletHitEnemyEvent);
}

void CollisionHandler::HandlePlayerEnemyCollision(EntityManager &entityManager, SystemManager &systemManager, EntityId playerEntityId, EntityId enemyEntityId)
{
	Tag *playerTag = entityManager.GetComponent<Tag>(playerEntityId);
	Tag *enemyTag = entityManager.GetComponent<Tag>(enemyEntityId);

	if (playerTag->GetEntityState() != EntityState::Alive)
		return;
	if (enemyTag->GetEntityState() == EntityState::Dead)
		return;

	playerTag->SetEntityState(EntityState::HitByEnemy);
	enemyTag->SetEntityState(EntityState::Dead);

	SoundManager::GetInstance().PlaySoundFromFile(Helper::PATH_TO_HURT);

	Event enemyHitPlayerEvent(EventType::EnemyHitPlayer, {enemyEntityId, playerEntityId});
	systemManager.SendEvent(enemyHitPlayerEvent);
}

void CollisionHandler::HandlePlayerAmmoPickupCollision(EntityManager &entityManager, SystemManager &systemManager, EntityId playerEntityId, EntityId ammoPickupEntityId)
{
	SoundManager::GetInstance().PlaySoundFromFile(Helper::PATH_TO_RELOAD);

	Event playerHitAmmoPickup(EventType::PlayerHitAmmoPickup, {playerEntityId, ammoPickupEntityId});
	systemManager.SendEvent(playerHitAmmoPickup);
}

void CollisionHandler::HandlePlayerHealthPickupCollision(EntityManager &entityManager, SystemManager &systemManager, EntityId playerEntityId, EntityId healthPickupEntityId)
{
	SoundManager::GetInstance().PlaySoundFromFile(Helper::PATH_TO_HEALTH_BOOST);

	Event playerHitHealthPickup(EventType::PlayerHitHealthPickup, {playerEntityId, healthPickupEntityId});
	systemManager.SendEvent(playerHitHealthPickup);
}

void CollisionHandler::HandlePlayerLightningPickupCollision(EntityManager &entityManager, SystemManager &systemManager, EntityId playerEntityId, EntityId lightningPickupEntityId)
{
	SoundManager::GetInstance().PlaySoundFromFile(Helper::PATH_TO_THUNDER);

	Event playerHitLightningPickup(EventType::PlayerHitLightningPickup, {playerEntityId, lightningPickupEntityId});
	systemManager.SendEvent(playerHitLightningPickup);
}
