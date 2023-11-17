#include "stdafx.h"
#include "../include/UpdateMovement.h"

void UpdateMovement::Update(EntityManager& entityManager, float deltaTime) {
    for (auto entity : entityManager.GetEntitiesWithComponents<Transform, Velocity>()) {
        auto transform = entityManager.GetComponent<Transform>(entity);
        auto velocity = entityManager.GetComponent<Velocity>(entity);

        if (transform && velocity) {
            transform->position.x += velocity->velocity.x * deltaTime;
            transform->position.y += velocity->velocity.y * deltaTime;
        }
    }
}
