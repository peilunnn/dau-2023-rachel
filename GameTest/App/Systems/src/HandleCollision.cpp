#include "stdafx.h"
#include "../include/HandleCollision.h"
#include <glm/glm.hpp>

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
            
            // Only proceed with detailed collision check if collision masks allow the two entities to collide
            int bitwiseAndResult = (static_cast<int>(collider1->collisionMask)) & (static_cast<int>(collider2->collisionType));
            if (bitwiseAndResult == 0)
                continue;

            if (IsColliding(transform1, collider1, transform2, collider2))
            {
                HandleCollisionEvent(entityManager, entity1, entity2);
            }
        }
    }
}

bool HandleCollision::IsColliding(std::shared_ptr<Transform> transform1, std::shared_ptr<Collider> collider1, std::shared_ptr<Transform> transform2, std::shared_ptr<Collider> collider2)
{
    if (!transform1 || !transform2 || !collider1 || !collider2)
        return false;

    // Calculate the distance between the centers of the two entities
    glm::vec2 posEntity1 = glm::vec2(transform1->position.x, transform1->position.y);
    glm::vec2 posEntity2 = glm::vec2(transform2->position.x, transform2->position.y);
    float distance = glm::distance(posEntity1, posEntity2);

    // Case 1 - Sphere-Sphere ie. bullet collide with enemy
    if (collider1->collisionShape == CollisionShape::SPHERE && collider2->collisionShape == CollisionShape::SPHERE) {
        float totalRadius = collider1->radius + collider2->radius;
        return distance < totalRadius;
    }
    // Case 2 - Capsule-Sphere ie. player collide with enemy
    // We will treat the capsule as a sphere for a very simple collision check
    else if (collider1->collisionShape == CollisionShape::CAPSULE || collider2->collisionShape == CollisionShape::SPHERE) {
        float capsuleRadius = (collider1->collisionShape == CollisionShape::CAPSULE) ? collider1->radius : collider2->radius;
        float sphereRadius = (collider1->collisionShape == CollisionShape::SPHERE) ? collider1->radius : collider2->radius;
        float totalRadius = capsuleRadius + sphereRadius;
        return distance < totalRadius;
    }
    return false;
}

void HandleCollision::HandleCollisionEvent(EntityManager& entityManager, Entity entity1, Entity entity2)
{
    // TODO: add logic to handle a collision event between entity1 and entity2
    // Eg. trigger enemy melting animation when bullet hits enemy
    std::shared_ptr<Tag> tag1 = entityManager.GetComponent<Tag>(entity1);
    std::shared_ptr<Tag> tag2 = entityManager.GetComponent<Tag>(entity2);
    std::string tag1String = Helper::GetEntityTypeString(tag1->entityType);
    std::string tag2String = Helper::GetEntityTypeString(tag2->entityType);

    Helper::Log("collision!");
    Helper::Log("entity1: ", tag1String);
    Helper::Log("entity2: ", tag2String);
}