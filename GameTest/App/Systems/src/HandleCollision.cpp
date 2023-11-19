#include "stdafx.h"
#include "../include/HandleCollision.h"

void HandleCollision::Update(EntityManager& entityManager, float deltaTime)
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

            // Check if collision categories match
            if (static_cast<int>(collider1->collisionMask) & static_cast<int>(collider2->collisionType) == 0)
                continue;

            // Check for actual collision based on shapes and positions
            if (IsColliding(transform1, collider1, transform2, collider2))
            {
                // Handle collision
                HandleCollisionEvent(entity1, entity2);
            }
        }
    }
}

bool HandleCollision::IsColliding(std::shared_ptr<Transform> transform1, std::shared_ptr<Collider> collider1,
    std::shared_ptr<Transform> transform2, std::shared_ptr<Collider> collider2)
{
    // TODO: add collision detection logic here based on collider shapes and transform positions
    // Eg. if both shapes are spheres, check the distance between the entities against the sum of their radii

    return false;
}

void HandleCollision::HandleCollisionEvent(Entity entity1, Entity entity2)
{
    // TODO: add logic to handle a collision event between entity1 and entity2
    // Eg. trigger enemy melting animation when bullet hits enemy
}