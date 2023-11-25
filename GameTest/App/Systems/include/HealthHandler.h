#ifndef HEALTH_HANDLER_H
#define HEALTH_HANDLER_H

#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Health.h"
#include "../../Systems/include/System.h"

class HealthHandler : public System
{
public:
    void Update(EntityManager& entityManager, float deltaTime) {
    }
};

#endif // HEALTH_HANDLER_H
