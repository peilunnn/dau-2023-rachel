#pragma once
#include "../../Components/include/Transform.h"
#include "../../Components/include/Collider.h"
using namespace std;

class CollisionHandler : public System
{
public:
	void Update(EntityManager& entityManager, SystemManager& systemManager, float deltaTime);
	bool IsColliding(shared_ptr<Transform> transform1, shared_ptr<Collider> collider1, shared_ptr<Transform> transform2, shared_ptr<Collider> collider2);
	void HandleCollisionEvent(EntityManager& entityManager, SystemManager& systemManager, EntityId entity1Id, EntityId entity2Id);
	SystemType GetSystemType() const override
	{
		return SystemType::CollisionHandler;
	}
};