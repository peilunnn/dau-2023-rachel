// UpdateMovementSystem.h
#ifndef UPDATE_MOVEMENT_SYSTEM_H
#define UPDATE_MOVEMENT_SYSTEM_H

#include "../../Entities/include/EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Velocity.h"

class UpdateMovementSystem {
public:
    void Update(EntityManager& entityManager, float deltaTime) {
    //    for (auto entity : entityManager.GetEntitiesWithComponents<Transform, Velocity>()) {
    //        auto& transform = entityManager.GetComponent<Transform>(entity);
    //        auto& velocity = entityManager.GetComponent<Velocity>(entity);

    //        transform.position += velocity.value * deltaTime; // Simple Euler integration
    //    }
    }
};

#endif // UPDATE_MOVEMENT_SYSTEM_H
