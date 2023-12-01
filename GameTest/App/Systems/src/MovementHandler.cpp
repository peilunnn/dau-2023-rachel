#include "stdafx.h"
#include "../include/MovementHandler.h"
#include "Components/include/Direction.h"
#include "Components/include/Tag.h"
#include "Components/include/Transform.h"
#include "Components/include/Renderable.h"
#include "Components/include/Velocity.h"
#include "Systems/include/ScreenHandler.h"
#include "Utilities/include/Helper.h"

void MovementHandler::Update(EntityManager& entityManager, float deltaTime)
{
	constexpr float screenWidth = ScreenHandler::SCREEN_WIDTH;
	constexpr float screenHeight = ScreenHandler::SCREEN_HEIGHT;

	for (auto entityId : entityManager.GetEntitiesWithComponents<Tag, Transform, Velocity>())
	{
		auto entityType = entityManager.GetComponent<Tag>(entityId)->GetEntityType();

		switch (entityType)
		{
		case EntityType::PLAYER:
			HandlePlayerMovement(entityManager, entityId, deltaTime);
			break;
		case EntityType::ENEMY:
			HandleEnemyMovement(entityManager, entityId, deltaTime);
			break;
		case EntityType::BULLET:
			HandleBulletMovement(entityManager, entityId, deltaTime);
			break;
		}
	}
}

void MovementHandler::HandlePlayerMovement(EntityManager& entityManager, EntityId entityId, float deltaTime)
{
	constexpr float topOffset = 60.0f;
	constexpr float multiplier = 0.25f;
	auto transform = entityManager.GetComponent<Transform>(entityId);
	auto velocity = entityManager.GetComponent<Velocity>(entityId);

	if (!(transform && velocity))
		return;

	float movementX = velocity->GetVelocity().x * deltaTime;
	float movementY = velocity->GetVelocity().y * deltaTime;
	float newX = transform->GetPosition().x + movementX;
	float newY = transform->GetPosition().y + movementY;

	auto sprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
	auto dimensions = Helper::GetSpriteDimensions(sprite, multiplier);

	float newXPos = max(ScreenHandler::SCREEN_LEFT + dimensions.adjustedWidth / 2,
		min(newX, ScreenHandler::SCREEN_RIGHT - dimensions.adjustedWidth / 2));

	float newYPos = max(ScreenHandler::SCREEN_TOP + dimensions.adjustedHeight / 2 + topOffset,
		min(newY, ScreenHandler::SCREEN_BOTTOM - dimensions.adjustedHeight / 2));

	glm::vec3 newPos = glm::vec3(newXPos, newYPos, transform->GetPosition().z);
	transform->SetPosition(newPos);
}

void MovementHandler::HandleEnemyMovement(EntityManager& entityManager, EntityId entityId, float deltaTime)
{
	constexpr float topOffset = 20.0f;
	constexpr float bottomOffset = -15.0f;
	constexpr float multiplier = 0.25f;
	auto transform = entityManager.GetComponent<Transform>(entityId);
	auto velocity = entityManager.GetComponent<Velocity>(entityId);
	auto direction = entityManager.GetComponent<Direction>(entityId);

	if (!(transform && velocity && direction))
		return;

	glm::vec2 movement = velocity->GetVelocity() * deltaTime;
	glm::vec3 newPos = transform->GetPosition() + glm::vec3(movement, 0.0f);
	transform->SetPosition(newPos);

	auto sprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
	auto dimensions = Helper::GetSpriteDimensions(sprite, multiplier);
	glm::vec2 currentVelocity = velocity->GetVelocity();

	if (!(direction->GetBounced()))
	{
		auto xPos = transform->GetPosition().x;
		auto yPos = transform->GetPosition().y;

		if (xPos <= ScreenHandler::SCREEN_LEFT + dimensions.adjustedWidth / 2 ||
			xPos >= ScreenHandler::SCREEN_RIGHT - dimensions.adjustedWidth / 2)
		{
			currentVelocity.x *= -1;
			velocity->SetVelocity(currentVelocity);
			direction->SetBounced(true);
		}
		if (yPos <= ScreenHandler::SCREEN_TOP + dimensions.adjustedHeight / 2 + topOffset ||
			yPos >= ScreenHandler::SCREEN_BOTTOM - dimensions.adjustedHeight / 2 - bottomOffset)
		{
			currentVelocity.y *= -1;
			velocity->SetVelocity(currentVelocity);
			direction->SetBounced(true);
		}
	}
	else
		direction->SetBounced(false);
}

void MovementHandler::HandleBulletMovement(EntityManager& entityManager, EntityId entityId, float deltaTime)
{
	auto transform = entityManager.GetComponent<Transform>(entityId);
	auto velocity = entityManager.GetComponent<Velocity>(entityId);

	if (!(transform && velocity))
		return;

	glm::vec2 movement = velocity->GetVelocity() * deltaTime;
	glm::vec3 newPos = transform->GetPosition() + glm::vec3(movement, 0.0f);
	transform->SetPosition(newPos);

	if (transform->GetPosition().x < ScreenHandler::SCREEN_LEFT || transform->GetPosition().x > ScreenHandler::SCREEN_RIGHT ||
		transform->GetPosition().y < ScreenHandler::SCREEN_TOP || transform->GetPosition().y > ScreenHandler::SCREEN_BOTTOM)
		entityManager.MarkEntityForDeletion(entityId);
}
