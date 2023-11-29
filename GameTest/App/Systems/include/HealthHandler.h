#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"

class HealthHandler : public System
{
public:
	void ProcessEnemyHitPlayer(EntityManager& entityManager);
	SystemType GetSystemType() const override
	{
		return SystemType::HealthHandler;
	}
};