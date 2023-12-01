#include "stdafx.h"
#include "Components/include/Tag.h"
#include "Components/include/Collider.h"
#include "Managers/include/EntityManager.h"
#include "Managers/include/SystemManager.h"
#include "Systems/include/Event.h"
#include "Systems/include/System.h"
#include "../include/CollisionHandler.h"
#include <set>
using glm::dot;
using glm::vec2;

void CollisionHandler::Update(EntityManager& entityManager, SystemManager& systemManager, float deltaTime)
{
	auto allEntities = entityManager.GetAllEntities();

	for (auto& i : allEntities)
	{
		auto iCollider = entityManager.GetComponent<Collider>(i);
		auto iTransform = entityManager.GetComponent<Transform>(i);

		if (!iCollider || !iTransform)
			continue;

		for (auto& j : allEntities)
		{
			if (i == j)
				continue;

			auto jCollider = entityManager.GetComponent<Collider>(j);
			auto jTransform = entityManager.GetComponent<Transform>(j);

			if (!jCollider || !jTransform)
				continue;

			// Only proceed with detailed collision check if collision masks allow the two entities to collide
			int bitwiseAndResult = (static_cast<int>(iCollider->GetCollisionMask())) & (static_cast<int>(jCollider->GetCollisionType()));
			if (bitwiseAndResult == 0)
				continue;

			if (IsColliding(iTransform, iCollider, jTransform, jCollider))
				HandleCollisionEvent(entityManager, systemManager, i, j);
		}
	}
}

bool CollisionHandler::IsColliding(shared_ptr<Transform> transform1, shared_ptr<Collider> collider1, shared_ptr<Transform> transform2, shared_ptr<Collider> collider2)
{
	if (!transform1 || !transform2 || !collider1 || !collider2)
		return false;

	vec2 firstEntityPos = vec2(transform1->GetPosition().x, transform1->GetPosition().y);
	vec2 secondEntityPos = vec2(transform2->GetPosition().x, transform2->GetPosition().y);
	vec2 diff = firstEntityPos - secondEntityPos;
	float squaredDistance = dot(diff, diff);

	float totalRadius = collider1->GetRadius() + collider2->GetRadius();
	float squaredTotalRadius = totalRadius * totalRadius;

	return squaredDistance < squaredTotalRadius;
}

void CollisionHandler::HandleCollisionEvent(EntityManager& entityManager, SystemManager& systemManager, EntityId firstEntityId, EntityId secondEntityId)
{
	auto firstEntityType = entityManager.GetComponent<Tag>(firstEntityId)->GetEntityType();
	auto secondEntityType = entityManager.GetComponent<Tag>(secondEntityId)->GetEntityType();

	// Case 1 - bullet-enemy
	if ((firstEntityType == EntityType::Bullet && secondEntityType == EntityType::Enemy) ||
		(firstEntityType == EntityType::Enemy && secondEntityType == EntityType::Bullet))
	{
		EntityId bulletEntity = (firstEntityType == EntityType::Bullet) ? firstEntityId : secondEntityId;
		EntityId enemyEntityId = (firstEntityType == EntityType::Enemy) ? firstEntityId : secondEntityId;
		auto enemyTag = entityManager.GetComponent<Tag>(enemyEntityId);

		if (enemyTag->GetEntityState() != EntityState::Alive)
			return;

		enemyTag->SetEntityState(EntityState::HitByBullet);
		Event bulletHitEnemyEvent(EventType::BulletHitEnemy, { bulletEntity, enemyEntityId });
		systemManager.SendEvent(bulletHitEnemyEvent);
	}

	// Case 2 - one is player, the other is enemy
	else if ((firstEntityType == EntityType::Player && secondEntityType == EntityType::Enemy) ||
		(firstEntityType == EntityType::Enemy && secondEntityType == EntityType::Player))
	{
		EntityId playerEntityId = (firstEntityType == EntityType::Player) ? firstEntityId : secondEntityId;
		EntityId enemyEntityId = (firstEntityType == EntityType::Enemy) ? firstEntityId : secondEntityId;
		auto playerTag = entityManager.GetComponent<Tag>(playerEntityId);

		if (playerTag->GetEntityState() != EntityState::Alive)
			return;

		playerTag->SetEntityState(EntityState::HitByEnemy);
		Event enemyHitPlayerEvent(EventType::EnemyHitPlayer, { playerEntityId, enemyEntityId });
		systemManager.SendEvent(enemyHitPlayerEvent);
	}

	// Case 3 - one is player, the other is reloadingCircle
	else if ((firstEntityType == EntityType::Player && secondEntityType == EntityType::ReloadingCircle) ||
		(firstEntityType == EntityType::ReloadingCircle && secondEntityType == EntityType::Player))
	{
		EntityId playerEntityId = (firstEntityType == EntityType::Player) ? firstEntityId : secondEntityId;
		EntityId reloadingCircleEntityId = (firstEntityType == EntityType::ReloadingCircle) ? firstEntityId : secondEntityId;
		auto playerTag = entityManager.GetComponent<Tag>(playerEntityId);

		Event playerHitReloadingCircleEvent(EventType::PlayerHitReloadingCircle, { playerEntityId, reloadingCircleEntityId });
		systemManager.SendEvent(playerHitReloadingCircleEvent);
	}
}