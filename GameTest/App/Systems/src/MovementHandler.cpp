#include "stdafx.h"
#include "../include/MovementHandler.h"
#include "Components/include/BounceDirection.h"
#include "Components/include/Tag.h"
#include "Components/include/Transform.h"
#include "Components/include/Renderable.h"
#include "Systems/include/ScreenHandler.h"
#include "Systems/include/ScreenHandler.h"
#include "Utilities/include/Helper.h"
using glm::vec3;

unordered_map<EntityId, vec2> MovementHandler::entityVelocities;

void MovementHandler::Update(EntityManager &entityManager, float deltaTime)
{
	constexpr float screenWidth = ScreenHandler::SCREEN_WIDTH;
	constexpr float screenHeight = ScreenHandler::SCREEN_HEIGHT;

	for (EntityId entityId : entityManager.GetEntitiesWithComponents<Tag, Transform>())
	{
		EntityType entityType = entityManager.GetComponent<Tag>(entityId)->GetEntityType();

		switch (entityType)
		{
		case EntityType::Player:
			HandlePlayerMovement(entityManager, entityId, deltaTime);
			break;
		case EntityType::Enemy:
			HandleEnemyMovement(entityManager, entityId, deltaTime);
			break;
		case EntityType::Bullet:
			HandleBulletMovement(entityManager, entityId, deltaTime);
			break;
		}
	}
}

vec2 MovementHandler::GetVelocity(EntityId entityId)
{
	auto it = entityVelocities.find(entityId);
	if (it != entityVelocities.end())
		return it->second;

	return vec2(0.0f);
}

void MovementHandler::SetVelocity(EntityId entityId, const vec2& velocity)
{
	entityVelocities[entityId] = velocity;
}

void MovementHandler::HandleEvent(const Event& event, EntityManager& entityManager, float deltaTime)
{
	Helper::Log("in MovementHandler::HandleEvent");

	if (event.eventType == "BulletHitEnemy") {
		HandleBulletHitEnemy(entityManager, event.entities[0], event.entities[1], deltaTime);
	}
}

void MovementHandler::HandleBulletHitEnemy(EntityManager& entityManager, EntityId firstEntityId, EntityId secondEntityId, float deltaTime)
{
	EntityId bulletEntityId, enemyEntityId;

	EntityType firstEntityType = entityManager.GetComponent<Tag>(firstEntityId)->GetEntityType();

	if (firstEntityType == EntityType::Bullet)
	{
		bulletEntityId = firstEntityId;
		enemyEntityId = secondEntityId;
	}
	else
	{
		bulletEntityId = secondEntityId;
		enemyEntityId = firstEntityId;
	}

	constexpr vec2 zeroVector = vec2(0.0f, 0.0f);
	SetVelocity(enemyEntityId, zeroVector);
}

void MovementHandler::HandlePlayerMovement(EntityManager &entityManager, EntityId entityId, float deltaTime)
{
	constexpr float topOffset = 60.0f;
	constexpr float multiplier = 0.25f;
	shared_ptr<Transform> transform = entityManager.GetComponent<Transform>(entityId);
	vec2 velocity = GetVelocity(entityId);

	float movementX = velocity.x * deltaTime;
	float movementY = velocity.y * deltaTime;
	float newX = transform->GetPosition().x + movementX;
	float newY = transform->GetPosition().y + movementY;

	shared_ptr<CSimpleSprite> sprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
	SpriteDimensions dimensions = Helper::GetSpriteDimensions(sprite, multiplier);

	float newXPos = max(ScreenHandler::SCREEN_LEFT + dimensions.adjustedWidth / 2,
						min(newX, ScreenHandler::SCREEN_RIGHT - dimensions.adjustedWidth / 2));

	float newYPos = max(ScreenHandler::SCREEN_TOP + dimensions.adjustedHeight / 2 + topOffset,
						min(newY, ScreenHandler::SCREEN_BOTTOM - dimensions.adjustedHeight / 2));

	vec3 newPos = vec3(newXPos, newYPos, transform->GetPosition().z);
	transform->SetPosition(newPos);
}

void MovementHandler::HandleEnemyMovement(EntityManager &entityManager, EntityId entityId, float deltaTime)
{
	constexpr float topOffset = 20.0f;
	constexpr float bottomOffset = -15.0f;
	constexpr float multiplier = 0.25f;
	shared_ptr<Transform> transform = entityManager.GetComponent<Transform>(entityId);
	shared_ptr<BounceDirection> bounceDirection = entityManager.GetComponent<BounceDirection>(entityId);
	vec2 velocity = GetVelocity(entityId);

	vec2 movement = velocity * deltaTime;
	vec3 newPos = transform->GetPosition() + vec3(movement, 0.0f);
	transform->SetPosition(newPos);

	shared_ptr<CSimpleSprite> sprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
	SpriteDimensions dimensions = Helper::GetSpriteDimensions(sprite, multiplier);

	if (!(bounceDirection->GetBounced()))
	{
		float xPos = transform->GetPosition().x;
		float yPos = transform->GetPosition().y;

		if (xPos <= ScreenHandler::SCREEN_LEFT + dimensions.adjustedWidth / 2 ||
			xPos >= ScreenHandler::SCREEN_RIGHT - dimensions.adjustedWidth / 2)
		{
			velocity.x *= -1;
			SetVelocity(entityId, velocity);
			bounceDirection->SetBounced(true);
		}
		if (yPos <= ScreenHandler::SCREEN_TOP + dimensions.adjustedHeight / 2 + topOffset ||
			yPos >= ScreenHandler::SCREEN_BOTTOM - dimensions.adjustedHeight / 2 - bottomOffset)
		{
			velocity.y *= -1;
			SetVelocity(entityId, velocity);
			bounceDirection->SetBounced(true);
		}
	}
	else
		bounceDirection->SetBounced(false);
}

void MovementHandler::HandleBulletMovement(EntityManager &entityManager, EntityId entityId, float deltaTime)
{
	shared_ptr<Transform> transform = entityManager.GetComponent<Transform>(entityId);
	vec2 velocity = GetVelocity(entityId);

	vec2 movement = velocity * deltaTime;
	vec3 newPos = transform->GetPosition() + vec3(movement, 0.0f);
	transform->SetPosition(newPos);

	if (transform->GetPosition().x < ScreenHandler::SCREEN_LEFT || transform->GetPosition().x > ScreenHandler::SCREEN_RIGHT ||
		transform->GetPosition().y < ScreenHandler::SCREEN_TOP || transform->GetPosition().y > ScreenHandler::SCREEN_BOTTOM)
		entityManager.MarkEntityForDeletion(entityId);
}
