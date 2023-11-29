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

	for (auto entity : entityManager.GetEntitiesWithComponents<Tag, Transform, Velocity>())
	{
		auto tag = entityManager.GetComponent<Tag>(entity);
		if (!tag)
			return;

		switch (tag->entityType)
		{
		case EntityType::PLAYER:
			HandlePlayerMovement(entityManager, entity, deltaTime);
			break;
		case EntityType::ENEMY:
			HandleEnemyMovement(entityManager, entity, deltaTime);
			break;
		case EntityType::BULLET:
			HandleBulletMovement(entityManager, entity, deltaTime);
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

	float movementX = velocity->velocity.x * deltaTime;
	float movementY = velocity->velocity.y * deltaTime;
	float newX = transform->position.x + movementX;
	float newY = transform->position.y + movementY;

	auto sprite = entityManager.GetComponent<Renderable>(entityId)->sprite;
	auto dimensions = Helper::GetSpriteDimensions(sprite, multiplier);

	transform->position.x = max(ScreenHandler::SCREEN_LEFT + dimensions.adjustedWidth / 2,
		min(newX, ScreenHandler::SCREEN_RIGHT - dimensions.adjustedWidth / 2));

	transform->position.y = max(ScreenHandler::SCREEN_TOP + dimensions.adjustedHeight / 2 + topOffset,
		min(newY, ScreenHandler::SCREEN_BOTTOM - dimensions.adjustedHeight / 2));
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

	glm::vec2 movement = velocity->velocity * deltaTime;
	transform->position.x += movement.x;
	transform->position.y += movement.y;

	auto sprite = entityManager.GetComponent<Renderable>(entityId)->sprite;
	auto dimensions = Helper::GetSpriteDimensions(sprite, multiplier);

	if (!(direction->bounced))
	{
		if (transform->position.x <= ScreenHandler::SCREEN_LEFT + dimensions.adjustedWidth / 2 ||
			transform->position.x >= ScreenHandler::SCREEN_RIGHT - dimensions.adjustedWidth / 2)
		{
			velocity->velocity.x *= -1;
			direction->bounced = true;
		}
		if (transform->position.y <= ScreenHandler::SCREEN_TOP + dimensions.adjustedHeight / 2 + topOffset ||
			transform->position.y >= ScreenHandler::SCREEN_BOTTOM - dimensions.adjustedHeight / 2 - bottomOffset)
		{
			velocity->velocity.y *= -1;
			direction->bounced = true;
		}
	}
	else if (transform->position.x > ScreenHandler::SCREEN_LEFT + dimensions.adjustedWidth / 2 &&
		transform->position.x < ScreenHandler::SCREEN_RIGHT - dimensions.adjustedWidth / 2 &&
		transform->position.y > ScreenHandler::SCREEN_TOP + dimensions.adjustedHeight / 2 &&
		transform->position.y < ScreenHandler::SCREEN_BOTTOM - dimensions.adjustedHeight / 2)
	{
		direction->bounced = false;
	}
}

void MovementHandler::HandleBulletMovement(EntityManager& entityManager, EntityId entityId, float deltaTime)
{
	auto transform = entityManager.GetComponent<Transform>(entityId);
	auto velocity = entityManager.GetComponent<Velocity>(entityId);

	if (!(transform && velocity))
		return;

	glm::vec2 movement = velocity->velocity * deltaTime;
	transform->position.x += movement.x;
	transform->position.y += movement.y;

	if (transform->position.x < ScreenHandler::SCREEN_LEFT || transform->position.x > ScreenHandler::SCREEN_RIGHT ||
		transform->position.y < ScreenHandler::SCREEN_TOP || transform->position.y > ScreenHandler::SCREEN_BOTTOM)
		entityManager.MarkEntityForDeletion(entityId);
}
