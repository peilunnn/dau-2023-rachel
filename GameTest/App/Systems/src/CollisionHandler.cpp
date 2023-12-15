#include "stdafx.h"
#include "Components/include/Collider.h"
#include "Components/include/Screen.h"
#include "Components/include/Tag.h"
#include "Managers/include/EntityManager.h"
#include "Managers/include/GameManager.h"
#include "Managers/include/SoundManager.h"
#include "Managers/include/SystemManager.h"
#include "Systems/include/CollisionHandler.h"
#include "Systems/include/Event.h"
#include "Systems/include/ShootingHandler.h"
#include "Utilities/include/Helper.h"
using glm::dot;
using glm::vec2;

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

	m_rootQuadtree.Clear();
	PopulateQuadtree(entityManager, allEntityIds);

	for (EntityId &entityId : allEntityIds)
	{
		Collider *collider = entityManager.GetComponent<Collider>(entityId);
		Transform *transform = entityManager.GetComponent<Transform>(entityId);

		if (!collider || !transform)
			continue;

		// Retrieve potential colliders' transforms and their associated entity id from the quadtree
		vector<QuadtreeElement> potentialElements = m_rootQuadtree.Retrieve(transform);
		for (QuadtreeElement &element : potentialElements)
		{
			// Ensure we are not comparing the entity with itself
			if (entityId == element.entityId)
				continue;

			Collider *potentialElementCollider = entityManager.GetComponent<Collider>(element.entityId);

			if (!potentialElementCollider)
				continue;

			// Only proceed with detailed collision check if collision masks allow the two entities to collide
			int bitwiseAndResult = (static_cast<int>(collider->GetCollisionMask())) & (static_cast<int>(potentialElementCollider->GetCollisionType()));
			if (bitwiseAndResult == 0)
				continue;

			// Check if the entities are colliding
			if (IsColliding(transform, collider, element.transform, potentialElementCollider))
				HandleCollisionEvent(entityId, element.entityId);
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

	// Case 3: player-ammoBox
	else if ((firstEntityType == EntityType::Player && secondEntityType == EntityType::AmmoBox) ||
			 (firstEntityType == EntityType::AmmoBox && secondEntityType == EntityType::Player))
	{
		EntityId playerEntityId = (firstEntityType == EntityType::Player) ? firstEntityId : secondEntityId;
		EntityId ammoBoxEntityId = (firstEntityType == EntityType::AmmoBox) ? firstEntityId : secondEntityId;
		HandlePlayerAmmoBoxCollision(systemManager, playerEntityId, ammoBoxEntityId);
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

void CollisionHandler::HandlePlayerAmmoBoxCollision(SystemManager &systemManager, EntityId playerEntityId, EntityId ammoBoxEntityId)
{
	SoundManager::GetInstance().PlaySoundFromFile(Helper::PATH_TO_RELOAD);
	ShootingHandler::GetInstance().HandlePlayerHitAmmoBox();
}
