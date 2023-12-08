#include "stdafx.h"
#include "Components/include/BounceDirection.h"
#include "Components/include/Renderable.h"
#include "Components/include/Screen.h"
#include "Components/include/Tag.h"
#include "Components/include/Transform.h"
#include "Components/include/Velocity.h"
#include "Managers/include/SystemManager.h"
#include "Systems/include/MovementHandler.h"
using glm::vec3;

void MovementHandler::Update(float deltaTime)
{
	EntityManager& entityManager = EntityManager::GetInstance();
	Screen& screen = Screen::GetInstance();
	const float screenWidth = screen.SCREEN_WIDTH;
	const float screenHeight = screen.SCREEN_HEIGHT;

	for (EntityId entityId : entityManager.GetEntitiesWithComponents<Tag, Transform>())
	{
		EntityType entityType = entityManager.GetComponent<Tag>(entityId)->GetEntityType();

		switch (entityType)
		{
		case EntityType::Player:
			HandlePlayerMovement(entityManager, screen, entityId, deltaTime);
			break;
		case EntityType::Enemy:
			HandleEnemyMovement(entityManager, screen, entityId, deltaTime);
			break;
		case EntityType::Bullet:
			HandleBulletMovement(entityManager, screen, entityId, deltaTime);
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

void MovementHandler::HandlePlayerMovement(EntityManager &entityManager, Screen& screen, EntityId entityId, float deltaTime)
{
	Transform* transform = entityManager.GetComponent<Transform>(entityId);
	Velocity* velocity = entityManager.GetComponent<Velocity>(entityId);
	CSimpleSprite* sprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
	constexpr float topOffset = -30.0f;
	constexpr float bottomOffset = 30.0f;
	const float widthBuffer = sprite->GetWidth() / 4.0f;
	const float heightBuffer = sprite->GetHeight() / 4.0f;

	vec2 currentVelocity = velocity->GetVelocity();
	float movementX = currentVelocity.x * deltaTime;
	float movementY = currentVelocity.y * deltaTime;
	float newX = transform->GetPosition().x + movementX;
	float newY = transform->GetPosition().y + movementY;

	float newXPos = max(screen.SCREEN_LEFT + widthBuffer,
						min(newX, screen.SCREEN_RIGHT - widthBuffer));

	float newYPos = max(screen.SCREEN_TOP + heightBuffer + bottomOffset,
						min(newY, screen.SCREEN_BOTTOM + heightBuffer + topOffset));

	vec3 newPos = vec3(newXPos, newYPos, transform->GetPosition().z);
	transform->SetPosition(newPos);
}

void MovementHandler::HandleEnemyMovement(EntityManager &entityManager, Screen& screen, EntityId entityId, float deltaTime)
{
	Transform* transform = entityManager.GetComponent<Transform>(entityId);
	BounceDirection* bounceDirection = entityManager.GetComponent<BounceDirection>(entityId);
	Velocity* velocity = entityManager.GetComponent<Velocity>(entityId);
	CSimpleSprite* sprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
	constexpr float topOffset = 20.0f;
	constexpr float bottomOffset = -30.0f;

	vec2 currentVelocity = velocity->GetVelocity();
	vec2 movement = currentVelocity * deltaTime;
	vec3 newPos = transform->GetPosition() + vec3(movement, 0.0f);
	transform->SetPosition(newPos);

	if (!(bounceDirection->GetBounced()))
	{
		const float xPos = transform->GetPosition().x;
		const float yPos = transform->GetPosition().y;
		const float widthBuffer = sprite->GetWidth() / 6.0f;
		const float heightBuffer = sprite->GetHeight() / 6.0f;

		if (xPos <= screen.SCREEN_LEFT + widthBuffer ||
			xPos >= screen.SCREEN_RIGHT - widthBuffer)
		{
			currentVelocity.x *= -1;
			velocity->SetVelocity(currentVelocity);
			bounceDirection->SetBounced(true);
		}
		if (yPos <= screen.SCREEN_TOP + heightBuffer + topOffset ||
			yPos >= screen.SCREEN_BOTTOM + heightBuffer + bottomOffset)
		{
			currentVelocity.y *= -1;
			velocity->SetVelocity(currentVelocity);
			bounceDirection->SetBounced(true);
		}
	}
	else
		bounceDirection->SetBounced(false);
}

void MovementHandler::HandleBulletMovement(EntityManager &entityManager, Screen& screen, EntityId entityId, float deltaTime)
{
	SystemManager& systemManager = SystemManager::GetInstance();
	Transform* transform = entityManager.GetComponent<Transform>(entityId);
	Velocity* velocity = entityManager.GetComponent<Velocity>(entityId);

	vec2 currentVelocity = velocity->GetVelocity();
	vec2 movement = currentVelocity * deltaTime;
	vec3 newPos = transform->GetPosition() + vec3(movement, 0.0f);
	transform->SetPosition(newPos);

	if (transform->GetPosition().x < screen.SCREEN_LEFT || transform->GetPosition().x > screen.SCREEN_RIGHT ||
		transform->GetPosition().y < screen.SCREEN_TOP || transform->GetPosition().y > screen.SCREEN_BOTTOM)
	{
		Event bulletOutOfBoundsEvent("BulletOutOfBounds", { entityId});
		systemManager.SendEvent(bulletOutOfBoundsEvent);
	}
}
