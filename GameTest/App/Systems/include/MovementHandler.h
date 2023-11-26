#pragma once
#include "../../Managers/include/EntityManager.h"
#include "../../Systems/include/System.h"

class MovementHandler : public System
{
public:
	void Update(EntityManager& entityManager, float deltaTime);
	void HandlePlayerMovement(EntityManager& entityManager, EntityId entityId, float deltaTime);
	void HandleEnemyMovement(EntityManager& entityManager, EntityId entityId, float deltaTime);
	void HandleBulletMovement(EntityManager& entityManager, EntityId entityId, float deltaTime);
	SystemType GetSystemType() const override
	{
		return SystemType::MovementHandler;
	}
};