#include "stdafx.h"
#include "../include/CollisionHandler.h"
#include <glm/glm.hpp>

void CollisionHandler::Update(EntityManager &entityManager, SystemManager &systemManager, float deltaTime)
{
    auto allEntities = entityManager.GetAllEntities();

    for (auto &entity1 : allEntities)
    {
        auto collider1 = entityManager.GetComponent<Collider>(entity1);
        auto transform1 = entityManager.GetComponent<Transform>(entity1);

        if (!collider1 || !transform1)
            continue;

        for (auto &entity2 : allEntities)
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
                HandleCollisionEvent(entityManager, systemManager, entity1, entity2);
            }
        }
    }
}

bool CollisionHandler::IsColliding(std::shared_ptr<Transform> transform1, std::shared_ptr<Collider> collider1, std::shared_ptr<Transform> transform2, std::shared_ptr<Collider> collider2)
{
    if (!transform1 || !transform2 || !collider1 || !collider2)
        return false;

    // Calculate the distance between the centers of the two entities
    glm::vec2 posEntity1 = glm::vec2(transform1->position.x, transform1->position.y);
    glm::vec2 posEntity2 = glm::vec2(transform2->position.x, transform2->position.y);
    float distance = glm::distance(posEntity1, posEntity2);

    // Case 1 - Sphere-Sphere ie. bullet collide with enemy
    if (collider1->collisionShape == CollisionShape::SPHERE && collider2->collisionShape == CollisionShape::SPHERE)
    {
        float totalRadius = collider1->radius + collider2->radius;
        return distance < totalRadius;
    }

    // Case 2 - Capsule-Sphere ie. player collide with enemy
    // We will treat the capsule as a sphere for a very simple collision check
    else if (collider1->collisionShape == CollisionShape::CAPSULE || collider2->collisionShape == CollisionShape::SPHERE)
    {
        float capsuleRadius = (collider1->collisionShape == CollisionShape::CAPSULE) ? collider1->radius : collider2->radius;
        float sphereRadius = (collider1->collisionShape == CollisionShape::SPHERE) ? collider1->radius : collider2->radius;
        float totalRadius = capsuleRadius + sphereRadius;
        return distance < totalRadius;
    }

    return false;
}

void CollisionHandler::HandleCollisionEvent(EntityManager &entityManager, SystemManager &systemManager, Entity entity1, Entity entity2)
{
    auto tag1 = entityManager.GetComponent<Tag>(entity1);
    auto tag2 = entityManager.GetComponent<Tag>(entity2);

    // Case 1 - bullet-enemy
    if ((tag1->entityType == EntityType::BULLET && tag2->entityType == EntityType::ENEMY) ||
        (tag1->entityType == EntityType::ENEMY && tag2->entityType == EntityType::BULLET))
    {
        Entity bulletEntity = (tag1->entityType == EntityType::BULLET) ? entity1 : entity2;
        Entity enemyEntity = (tag1->entityType == EntityType::ENEMY) ? entity1 : entity2;
        auto enemyTag = entityManager.GetComponent<Tag>(enemyEntity);

        if (enemyTag->state != EntityState::ALIVE)
        {
            return;
        }
        
        enemyTag->state = EntityState::HIT_BY_BULLET;
        event.type = SystemManager::SystemEvent::EventType::BulletHitEnemy;
        event.entities.push_back(entity1);
        event.entities.push_back(entity2);
        event.newEnemiesCreated = false;
        systemManager.SendEvent(event);
        event.entities.clear();
    }
    // Case 2 - one is player, the other is enemy
}