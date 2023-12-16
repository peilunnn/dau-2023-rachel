#pragma once
#include "Components/include/Collider.h"
#include "Components/include/Screen.h"
#include "Components/include/Transform.h"
#include "Managers/include/SystemManager.h"
#include "Utilities/include/QuadtreeNode.h"
using namespace std;

class CollisionHandler : public System
{
public:
    static CollisionHandler &GetInstance();
    CollisionHandler(CollisionHandler const &) = delete;
    void operator=(CollisionHandler const &) = delete;

    void Update(float deltaTime);

private:
    CollisionHandler::CollisionHandler()
        : m_rootQuadtree(QuadtreeNode(0, 0, 0, Screen::GetInstance().SCREEN_WIDTH, Screen::GetInstance().SCREEN_HEIGHT)) {}

    QuadtreeNode m_rootQuadtree;
    static map<EntityType, set<EntityType>> m_collisionRules;

    void PopulateQuadtree(EntityManager &entityManager, vector<EntityId> allEntityIds);
    bool IsColliding(Transform *firstTransform, Collider *firstCollider, Transform *secondTransform, Collider *secondCollider);
    void HandleCollisionEvent(EntityId firstEntityId, EntityId secondEntityId);
    void HandleBulletEnemyCollision(EntityManager &entityManager, SystemManager &systemManager, EntityId bulletEntityId, EntityId enemyEntityId);
    void HandlePlayerEnemyCollision(EntityManager &entityManager, SystemManager &systemManager, EntityId playerEntityId, EntityId enemyEntityId);
    void HandlePlayerAmmoPickupCollision(EntityManager& entityManager, SystemManager& systemManager, EntityId playerEntityId, EntityId ammoPickupEntityId);
    void HandlePlayerHealthPickupCollision(EntityManager& entityManager, SystemManager& systemManager, EntityId playerEntityId, EntityId healthPickupEntityId);
    void HandlePlayerLightningPickupCollision(EntityManager& entityManager, SystemManager& systemManager, EntityId playerEntityId, EntityId lightningPickupEntityId);
};
