#include "stdafx.h"
#include "Components/include/Collider.h"
#include "Managers/include/EntityManager.h"
#include "Managers/include/SystemManager.h"
#include "Systems/include/Event.h"
#include "Systems/include/System.h"
#include "Utilities/include/Helper.h"
#include "../include/CollisionHandler.h"
#include <glm/glm.hpp>
#include <set>

void CollisionHandler::Update(EntityManager& entityManager, SystemManager& systemManager, float deltaTime)
{
	auto allEntities = entityManager.GetAllEntities();

	for (auto& entity1 : allEntities)
	{
		auto collider1 = entityManager.GetComponent<Collider>(entity1);
		auto transform1 = entityManager.GetComponent<Transform>(entity1);

		if (!collider1 || !transform1)
			continue;

		for (auto& entity2 : allEntities)
		{
			if (entity1 == entity2)
				continue;

			auto collider2 = entityManager.GetComponent<Collider>(entity2);
			auto transform2 = entityManager.GetComponent<Transform>(entity2);

			if (!collider2 || !transform2)
				continue;

			// Only proceed with detailed collision check if collision masks allow the two entities to collide
			int bitwiseAndResult = (static_cast<int>(collider1->GetCollisionMask())) & (static_cast<int>(collider2->GetCollisionType()));
			if (bitwiseAndResult == 0)
				continue;

			if (IsColliding(transform1, collider1, transform2, collider2))
				HandleCollisionEvent(entityManager, systemManager, entity1, entity2);
		}
	}
}

bool CollisionHandler::IsColliding(shared_ptr<Transform> transform1, shared_ptr<Collider> collider1, shared_ptr<Transform> transform2, shared_ptr<Collider> collider2)
{
	if (!transform1 || !transform2 || !collider1 || !collider2)
		return false;

	// Calculate the distance between the centers of the two entities
	glm::vec2 posEntity1 = glm::vec2(transform1->GetPosition().x, transform1->GetPosition().y);
	glm::vec2 posEntity2 = glm::vec2(transform2->GetPosition().x, transform2->GetPosition().y);
	float distance = glm::distance(posEntity1, posEntity2);

	// Case 1 - Sphere-Sphere ie. bullet collide with enemy, or player collide with reloadingCircle
	if (collider1->GetCollisionShape() == CollisionShape::SPHERE && collider2->GetCollisionShape() == CollisionShape::SPHERE)
	{
		float totalRadius = collider1->GetRadius() + collider2->GetRadius();
		return distance < totalRadius;
	}

	// Case 2 - Capsule-Sphere ie. player collide with enemy
	// We will treat the capsule as a sphere for a very simple collision check
	else if (collider1->GetCollisionShape() == CollisionShape::CAPSULE || collider2->GetCollisionShape()  == CollisionShape::SPHERE)
	{
		float capsuleRadius = (collider1->GetCollisionShape() == CollisionShape::CAPSULE) ? collider1->GetRadius() : collider2->GetRadius();
		float sphereRadius = (collider1->GetCollisionShape() == CollisionShape::SPHERE) ? collider1->GetRadius() : collider2->GetRadius();
		float totalRadius = capsuleRadius + sphereRadius;
		return distance < totalRadius;
	}

	return false;
}

void CollisionHandler::HandleCollisionEvent(EntityManager& entityManager, SystemManager& systemManager, EntityId entity1Id, EntityId entity2Id)
{
	auto tag1 = entityManager.GetComponent<Tag>(entity1Id);
	auto tag2 = entityManager.GetComponent<Tag>(entity2Id);

	// Case 1 - bullet-enemy
	if ((tag1->GetEntityType() == EntityType::BULLET && tag2->GetEntityType() == EntityType::ENEMY) ||
		(tag1->GetEntityType() == EntityType::ENEMY && tag2->GetEntityType() == EntityType::BULLET))
	{
		EntityId bulletEntity = (tag1->GetEntityType() == EntityType::BULLET) ? entity1Id : entity2Id;
		EntityId enemyEntity = (tag1->GetEntityType() == EntityType::ENEMY) ? entity1Id : entity2Id;
		auto enemyTag = entityManager.GetComponent<Tag>(enemyEntity);

		if (enemyTag->GetEntityState() != EntityState::ALIVE)
			return;

		enemyTag->SetEntityState(EntityState::HIT_BY_BULLET);
		Event bulletHitEnemyEvent(EventType::BulletHitEnemy, { bulletEntity, enemyEntity });
		systemManager.SendEvent(bulletHitEnemyEvent);
	}

	// Case 2 - one is player, the other is enemy
	else if ((tag1->GetEntityType() == EntityType::PLAYER && tag2->GetEntityType() == EntityType::ENEMY) ||
		(tag1->GetEntityType() == EntityType::ENEMY && tag2->GetEntityType() == EntityType::PLAYER))
	{
		EntityId playerEntityId = (tag1->GetEntityType() == EntityType::PLAYER) ? entity1Id : entity2Id;
		EntityId enemyEntity = (tag1->GetEntityType() == EntityType::ENEMY) ? entity1Id : entity2Id;
		auto playerTag = entityManager.GetComponent<Tag>(playerEntityId);

		if (playerTag->GetEntityState() != EntityState::ALIVE)
			return;

		playerTag->SetEntityState(EntityState::HIT_BY_ENEMY);
		Event enemyHitPlayerEvent(EventType::EnemyHitPlayer, { playerEntityId, enemyEntity });
		systemManager.SendEvent(enemyHitPlayerEvent);
	}

	// Case 3 - one is player, the other is reloadingCircle
	else if ((tag1->GetEntityType() == EntityType::PLAYER && tag2->GetEntityType() == EntityType::RELOADING_CIRCLE) ||
		(tag1->GetEntityType() == EntityType::RELOADING_CIRCLE && tag2->GetEntityType() == EntityType::PLAYER))
	{
		EntityId playerEntityId = (tag1->GetEntityType() == EntityType::PLAYER) ? entity1Id : entity2Id;
		EntityId reloadingCircleEntity = (tag1->GetEntityType() == EntityType::RELOADING_CIRCLE) ? entity1Id : entity2Id;
		auto playerTag = entityManager.GetComponent<Tag>(playerEntityId);

		Event playerHitReloadingCircleEvent(EventType::PlayerHitReloadingCircle, { playerEntityId, reloadingCircleEntity });
		systemManager.SendEvent(playerHitReloadingCircleEvent);
	}
}