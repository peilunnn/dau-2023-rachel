#ifndef UPDATE_MOVEMENT_H
#define UPDATE_MOVEMENT_H

#include "../../Entities/include/EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Velocity.h"

class UpdateMovement {
public:
    void Update(EntityManager& entityManager, float deltaTime);
};

#endif // UPDATE_MOVEMENT_H
