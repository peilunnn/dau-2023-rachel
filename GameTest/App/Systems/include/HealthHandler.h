#ifndef HEALTH_HANDLER_H
#define HEALTH_HANDLER_H

#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Health.h"
#include "../../Systems/include/System.h"
#include <algorithm>

class HealthHandler : public System
{
public:
    void ProcessEnemyHitPlayer(EntityManager& entityManager);
	Type GetSystemType() const override {
		return System::Type::HealthHandler;
	}
};

#endif // HEALTH_HANDLER_H
