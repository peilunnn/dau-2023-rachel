#include "stdafx.h"
#include "../include/EntityManager.h"

Entity EntityManager::nextEntityId = 1;

Entity EntityManager::CreatePlayerEntity(CSimpleSprite* playerSprite)
{
	Entity playerEntityId = CreateEntity();

	// Create components
	auto playerTransform = std::make_shared<Transform>(glm::vec3(400.0f, 400.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	auto playerRenderable = std::make_shared<Renderable>(playerSprite);

	auto playerCollider = std::make_shared<Collider>();
	playerCollider->collisionShape = CollisionShape::SPHERE;
	playerCollider->collisionMask = 1;

	auto playerVelocity = std::make_shared<Velocity>(0.0f, 0.0f);

	auto playerDirection = std::make_shared<Direction>();
	playerDirection->direction = glm::vec2(0.0f, -1.0f);

	auto playerHealth = std::make_shared<Health>();
	auto playerAnimation = std::make_shared<Animation>();

	// Add components
	EntityManager::AddComponent(playerEntityId, playerTransform);
	EntityManager::AddComponent(playerEntityId, playerRenderable);
	EntityManager::AddComponent(playerEntityId, playerCollider);
	EntityManager::AddComponent(playerEntityId, playerVelocity);
	EntityManager::AddComponent(playerEntityId, playerDirection);
	EntityManager::AddComponent(playerEntityId, playerHealth);
	EntityManager::AddComponent(playerEntityId, playerAnimation);

	return playerEntityId;
}
