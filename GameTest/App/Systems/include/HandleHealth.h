#ifndef HANDLE_HEALTH_H
#define HANDLE_HEALTH_H

#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Health.h"
#include "../../Components/include/Damage.h"
#include "../../Systems/include/System.h"

class HandleHealth : public System
{
public:
    void Update(EntityManager& entityManager, float deltaTime) {
    }
};

#endif // HANDLE_HEALTH_H
