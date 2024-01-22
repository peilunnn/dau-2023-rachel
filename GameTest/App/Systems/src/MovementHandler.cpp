#include "stdafx.h"
#include "Components/include/BounceDirection.h"
#include "Components/include/EnemyType.h"
#include "Components/include/Renderable.h"
#include "Components/include/Screen.h"
#include "Components/include/Tag.h"
#include "Components/include/Transform.h"
#include "Components/include/Velocity.h"
#include "Managers/include/GameManager.h"
#include "Managers/include/SystemManager.h"
#include "Systems/include/MovementHandler.h"
#include "ParticleSystem/include/ParticleManager.h"
using glm::vec3;
using std::min;
using std::max;

MovementHandler& MovementHandler::GetInstance()
{
	static MovementHandler instance;
	return instance;
}

void MovementHandler::Update(float deltaTime)
{
	GameManager& gameManager = GameManager::GetInstance();
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
			{
				Tag* playerTag = entityManager.GetComponent<Tag>(entityId);

				if (playerTag->GetEntityState() != EntityState::Alive)
					return;

				UpdatePlayerMovement(entityManager, screen, entityId, deltaTime);
				break;
			}
			case EntityType::Enemy:
				UpdateEnemyMovement(entityManager, screen, entityId, deltaTime);
				break;
			case EntityType::Bullet:
				UpdateBulletMovement(entityManager, screen, entityId, deltaTime);
				break;
		}
	}

	// TODO: remove this
	//UpdateParticleMovement(deltaTime);
}

void MovementHandler::HandleEvent(const Event& event, float deltaTime)
{
	EntityManager& entityManager = EntityManager::GetInstance();

	if (event.GetEventType() == EventType::BulletHitEnemy) {
		HandleBulletHitEnemy(entityManager, event.GetEntities()[1], deltaTime);
	}
}

void MovementHandler::HandleBulletHitEnemy(EntityManager& entityManager, EntityId enemyEntityId, float deltaTime)
{
	Velocity* velocity = entityManager.GetComponent<Velocity>(enemyEntityId);
	velocity->SetVelocity(ZERO_VELOCITY);
}

void MovementHandler::UpdatePlayerMovement(EntityManager &entityManager, Screen& screen, EntityId entityId, float deltaTime)
{
	Transform* transform = entityManager.GetComponent<Transform>(entityId);
	Velocity* velocity = entityManager.GetComponent<Velocity>(entityId);
	CSimpleSprite* sprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
	const float widthBuffer = sprite->GetWidth() / 4.0f;
	const float heightBuffer = sprite->GetHeight() / 4.0f;

	vec2 currentVelocity = velocity->GetVelocity();
	float movementX = currentVelocity.x * deltaTime;
	float movementY = currentVelocity.y * deltaTime;
	float newX = transform->GetPosition().x + movementX;
	float newY = transform->GetPosition().y + movementY;

	float newXPos = max(screen.BORDER_LEFT_SCREEN_COORD + widthBuffer,
						min(newX, screen.BORDER_RIGHT_SCREEN_COORD - widthBuffer));
	float newYPos = max(screen.BORDER_TOP_SCREEN_COORD + heightBuffer + PLAYER_BOTTOM_OFFSET,
						min(newY, screen.BORDER_BOTTOM_SCREEN_COORD + heightBuffer + PLAYER_TOP_OFFSET));
	vec3 newPos = vec3(newXPos, newYPos, transform->GetPosition().z);
	transform->SetPosition(newPos);
}

void MovementHandler::UpdateEnemyMovement(EntityManager& entityManager, Screen& screen, EntityId entityId, float deltaTime)
{
	EnemyType* enemyBehaviorComponent = entityManager.GetComponent<EnemyType>(entityId);

	if (enemyBehaviorComponent->GetMovementType() == EnemyMovementType::Standard)
		UpdateStandardEnemyMovement(entityManager, screen, entityId, deltaTime);
	else
		UpdateHomingEnemyMovement(entityManager, screen, entityId, deltaTime);
}

void MovementHandler::UpdateStandardEnemyMovement(EntityManager &entityManager, Screen& screen, EntityId entityId, float deltaTime)
{
	Transform* transform = entityManager.GetComponent<Transform>(entityId);
	BounceDirection* bounceDirection = entityManager.GetComponent<BounceDirection>(entityId);
	Velocity* velocity = entityManager.GetComponent<Velocity>(entityId);
	CSimpleSprite* sprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();

	vec2 currentVelocity = velocity->GetVelocity();
	vec2 movement = currentVelocity * deltaTime;
	vec3 newPos = transform->GetPosition() + vec3(movement, ZERO_POS);
	transform->SetPosition(newPos);

	if (!(bounceDirection->GetBounced()))
	{
		const float xPos = transform->GetPosition().x;
		const float yPos = transform->GetPosition().y;
		const float widthBuffer = sprite->GetWidth() / 6.0f;
		const float heightBuffer = sprite->GetHeight() / 6.0f;

		if (xPos <= screen.BORDER_LEFT_SCREEN_COORD + widthBuffer ||
			xPos >= screen.BORDER_RIGHT_SCREEN_COORD - widthBuffer)
		{
			currentVelocity.x *= -1;
			velocity->SetVelocity(currentVelocity);
			bounceDirection->SetBounced(true);
		}
		if (yPos <= screen.BORDER_TOP_SCREEN_COORD + heightBuffer + ENEMY_TOP_OFFSET ||
			yPos >= screen.BORDER_BOTTOM_SCREEN_COORD + heightBuffer + ENEMY_BOTTOM_OFFSET)
		{
			currentVelocity.y *= -1;
			velocity->SetVelocity(currentVelocity);
			bounceDirection->SetBounced(true);
		}
	}
	else
		bounceDirection->SetBounced(false);
}

void MovementHandler::UpdateHomingEnemyMovement(EntityManager& entityManager, Screen& screen, EntityId entityId, float deltaTime)
{
	Transform* enemyTransform = entityManager.GetComponent<Transform>(entityId);
	Velocity* enemyVelocity = entityManager.GetComponent<Velocity>(entityId);
	EntityId playerEntityId = entityManager.GetPlayerEntityId();
	Transform* playerTransform = entityManager.GetComponent<Transform>(playerEntityId);

	vec3 direction = playerTransform->GetPosition() - enemyTransform->GetPosition();
	vec2 normalizedDirection = normalize(vec2(direction.x, direction.y));
	enemyVelocity->SetVelocity(normalizedDirection * HOMING_SPEED);

	vec2 enemyMovement = enemyVelocity->GetVelocity() * deltaTime;
	vec3 enemyNewPos = enemyTransform->GetPosition() + vec3(enemyMovement, ZERO_POS);
	enemyTransform->SetPosition(enemyNewPos);
}

//void MovementHandler::UpdateParticleMovement(float deltaTime)
//{
//	EntityManager& entityManager = EntityManager::GetInstance();
//	ParticleManager& particleHandler = ParticleManager::GetInstance();
//
//	unordered_map<ParticleType, vector<EntityId>>& activeParticles = particleHandler.GetActiveParticles();
//
//	for (auto& pair : activeParticles) {
//		vector<EntityId>& particleEntityIds = pair.second;
//		for (EntityId particleEntityId : particleEntityIds) 
//		{
//			Transform* particleTransform = entityManager.GetComponent<Transform>(particleEntityId);
//			Velocity* particleVelocity = entityManager.GetComponent<Velocity>(particleEntityId);
//
//			vec3 newPosition = particleTransform->GetPosition() + vec3(particleVelocity->GetVelocity(), ZERO_POS) * deltaTime;
//			particleTransform->SetPosition(newPosition);
//		}
//	}
//}

void MovementHandler::UpdateBulletMovement(EntityManager &entityManager, Screen& screen, EntityId entityId, float deltaTime)
{
	SystemManager& systemManager = SystemManager::GetInstance();
	Transform* transform = entityManager.GetComponent<Transform>(entityId);
	Velocity* velocity = entityManager.GetComponent<Velocity>(entityId);

	vec2 currentVelocity = velocity->GetVelocity();
	vec2 movement = currentVelocity * deltaTime;
	vec3 newPos = transform->GetPosition() + vec3(movement, ZERO_POS);
	transform->SetPosition(newPos);

	if (transform->GetPosition().x < screen.BORDER_LEFT_SCREEN_COORD || transform->GetPosition().x > screen.BORDER_RIGHT_SCREEN_COORD ||
		transform->GetPosition().y < screen.BORDER_TOP_SCREEN_COORD || transform->GetPosition().y > screen.BORDER_BOTTOM_SCREEN_COORD)
	{
		Event bulletOutOfBoundsEvent(EventType::BulletOutOfBounds, { entityId});
		systemManager.SendEvent(bulletOutOfBoundsEvent);
	}
}
