#include "stdafx.h"
#include "../include/MovementHandler.h"
#include "Components/include/BounceDirection.h"
#include "Components/include/Tag.h"
#include "Components/include/Transform.h"
#include "Components/include/Renderable.h"
#include "Components/include/Velocity.h"
#include "Utilities/include/Helper.h"
using glm::vec3;

void MovementHandler::Update(float deltaTime)
{
	EntityManager& entityManager = EntityManager::GetInstance();
	ScreenHandler& screenHandler = ScreenHandler::GetInstance();
	const float screenWidth = screenHandler.SCREEN_WIDTH;
	const float screenHeight = screenHandler.SCREEN_HEIGHT;

	for (EntityId entityId : entityManager.GetEntitiesWithComponents<Tag, Transform>())
	{
		EntityType entityType = entityManager.GetComponent<Tag>(entityId)->GetEntityType();

		switch (entityType)
		{
		case EntityType::Player:
			HandlePlayerMovement(entityManager, screenHandler, entityId, deltaTime);
			break;
		case EntityType::Enemy:
			HandleEnemyMovement(entityManager, screenHandler, entityId, deltaTime);
			break;
		case EntityType::Bullet:
			HandleBulletMovement(entityManager, screenHandler, entityId, deltaTime);
			break;
		}
	}
}

void MovementHandler::HandleEvent(const Event& event, float deltaTime)
{
	EntityManager& entityManager = EntityManager::GetInstance();

	if (event.GetEventType() == "BulletHitEnemy") {
		HandleBulletHitEnemy(entityManager, event.GetEntities()[1], deltaTime);
	}
}

void MovementHandler::HandleBulletHitEnemy(EntityManager& entityManager, EntityId enemyEntityId, float deltaTime)
{
	constexpr vec2 zeroVector = vec2(0.0f, 0.0f);
	Velocity* velocity = entityManager.GetComponent<Velocity>(enemyEntityId);
	velocity->SetVelocity(zeroVector);
}

void MovementHandler::HandlePlayerMovement(EntityManager &entityManager, ScreenHandler& screenHandler, EntityId entityId, float deltaTime)
{
	constexpr float topOffset = 60.0f;
	constexpr float multiplier = 0.25f;
	Transform* transform = entityManager.GetComponent<Transform>(entityId);
	Velocity* velocity = entityManager.GetComponent<Velocity>(entityId);

	vec2 currentVelocity = velocity->GetVelocity();
	float movementX = currentVelocity.x * deltaTime;
	float movementY = currentVelocity.y * deltaTime;
	float newX = transform->GetPosition().x + movementX;
	float newY = transform->GetPosition().y + movementY;

	CSimpleSprite* sprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
	SpriteDimensions dimensions = Helper::GetSpriteDimensions(sprite, multiplier);

	float newXPos = max(screenHandler.SCREEN_LEFT + dimensions.adjustedWidth / 2,
						min(newX, screenHandler.SCREEN_RIGHT - dimensions.adjustedWidth / 2));

	float newYPos = max(screenHandler.SCREEN_TOP + dimensions.adjustedHeight / 2 + topOffset,
						min(newY, screenHandler.SCREEN_BOTTOM - dimensions.adjustedHeight / 2));

	vec3 newPos = vec3(newXPos, newYPos, transform->GetPosition().z);
	transform->SetPosition(newPos);
}

void MovementHandler::HandleEnemyMovement(EntityManager &entityManager, ScreenHandler& screenHandler, EntityId entityId, float deltaTime)
{
	constexpr float topOffset = 20.0f;
	constexpr float bottomOffset = -15.0f;
	constexpr float multiplier = 0.25f;
	Transform* transform = entityManager.GetComponent<Transform>(entityId);
	BounceDirection* bounceDirection = entityManager.GetComponent<BounceDirection>(entityId);
	Velocity* velocity = entityManager.GetComponent<Velocity>(entityId);

	vec2 currentVelocity = velocity->GetVelocity();
	vec2 movement = currentVelocity * deltaTime;
	vec3 newPos = transform->GetPosition() + vec3(movement, 0.0f);
	transform->SetPosition(newPos);

	CSimpleSprite* sprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
	SpriteDimensions dimensions = Helper::GetSpriteDimensions(sprite, multiplier);

	if (!(bounceDirection->GetBounced()))
	{
		float xPos = transform->GetPosition().x;
		float yPos = transform->GetPosition().y;

		if (xPos <= screenHandler.SCREEN_LEFT + dimensions.adjustedWidth / 2 ||
			xPos >= screenHandler.SCREEN_RIGHT - dimensions.adjustedWidth / 2)
		{
			currentVelocity.x *= -1;
			velocity->SetVelocity(currentVelocity);
			bounceDirection->SetBounced(true);
		}
		if (yPos <= screenHandler.SCREEN_TOP + dimensions.adjustedHeight / 2 + topOffset ||
			yPos >= screenHandler.SCREEN_BOTTOM - dimensions.adjustedHeight / 2 - bottomOffset)
		{
			currentVelocity.y *= -1;
			velocity->SetVelocity(currentVelocity);
			bounceDirection->SetBounced(true);
		}
	}
	else
		bounceDirection->SetBounced(false);
}

void MovementHandler::HandleBulletMovement(EntityManager &entityManager, ScreenHandler& screenHandler, EntityId entityId, float deltaTime)
{
	Transform* transform = entityManager.GetComponent<Transform>(entityId);
	Velocity* velocity = entityManager.GetComponent<Velocity>(entityId);

	vec2 currentVelocity = velocity->GetVelocity();
	vec2 movement = currentVelocity * deltaTime;
	vec3 newPos = transform->GetPosition() + vec3(movement, 0.0f);
	transform->SetPosition(newPos);

	if (transform->GetPosition().x < screenHandler.SCREEN_LEFT || transform->GetPosition().x > screenHandler.SCREEN_RIGHT ||
		transform->GetPosition().y < screenHandler.SCREEN_TOP || transform->GetPosition().y > screenHandler.SCREEN_BOTTOM)
		entityManager.MarkEntityForDeletion(entityId);
}
