#include "stdafx.h"
#include "Components/include/BounceDirection.h"
#include "Components/include/EnemyBehavior.h"
#include "Components/include/Renderable.h"
#include "Components/include/Screen.h"
#include "Components/include/Tag.h"
#include "Components/include/Transform.h"
#include "Components/include/Velocity.h"
#include "Managers/include/GameManager.h"
#include "Managers/include/SystemManager.h"
#include "Systems/include/MovementHandler.h"
#include "Systems/include/ParticleHandler.h"
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

	if (gameManager.GetCurrentGameState() == GameState::Paused)
		return;

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

	UpdateParticleMovement(deltaTime);
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
	constexpr vec2 zeroVelocity = vec2(0.0f, 0.0f);
	velocity->SetVelocity(zeroVelocity);
}

void MovementHandler::UpdatePlayerMovement(EntityManager &entityManager, Screen& screen, EntityId entityId, float deltaTime)
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

	float newXPos = max(screen.BORDER_LEFT_SCREEN_COORD + widthBuffer,
						min(newX, screen.BORDER_RIGHT_SCREEN_COORD - widthBuffer));
	float newYPos = max(screen.BORDER_TOP_SCREEN_COORD + heightBuffer + bottomOffset,
						min(newY, screen.BORDER_BOTTOM_SCREEN_COORD + heightBuffer + topOffset));
	vec3 newPos = vec3(newXPos, newYPos, transform->GetPosition().z);
	transform->SetPosition(newPos);
}

void MovementHandler::UpdateEnemyMovement(EntityManager& entityManager, Screen& screen, EntityId entityId, float deltaTime)
{
	EnemyBehavior* enemyBehaviorComponent = entityManager.GetComponent<EnemyBehavior>(entityId);

	if (enemyBehaviorComponent->GetBehaviorType() == EnemyBehaviorType::Standard)
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

		if (xPos <= screen.BORDER_LEFT_SCREEN_COORD + widthBuffer ||
			xPos >= screen.BORDER_RIGHT_SCREEN_COORD - widthBuffer)
		{
			currentVelocity.x *= -1;
			velocity->SetVelocity(currentVelocity);
			bounceDirection->SetBounced(true);
		}
		if (yPos <= screen.BORDER_TOP_SCREEN_COORD + heightBuffer + topOffset ||
			yPos >= screen.BORDER_BOTTOM_SCREEN_COORD + heightBuffer + bottomOffset)
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
	vec3 enemyNewPos = enemyTransform->GetPosition() + vec3(enemyMovement, 0.0f);
	enemyTransform->SetPosition(enemyNewPos);
}

void MovementHandler::UpdateParticleMovement(float deltaTime)
{
	EntityManager& entityManager = EntityManager::GetInstance();
	ParticleHandler& particleHandler = ParticleHandler::GetInstance();

	unordered_map<ParticleType, vector<EntityId>>& activeParticles = particleHandler.GetActiveParticles();

	for (auto& pair : activeParticles) {
		vector<EntityId>& particleEntityIds = pair.second;
		for (EntityId particleEntityId : particleEntityIds) 
		{
			Transform* particleTransform = entityManager.GetComponent<Transform>(particleEntityId);
			Velocity* particleVelocity = entityManager.GetComponent<Velocity>(particleEntityId);

			vec3 newPosition = particleTransform->GetPosition() + vec3(particleVelocity->GetVelocity(), 0.0f) * deltaTime;
			particleTransform->SetPosition(newPosition);
		}
	}
}

void MovementHandler::UpdateBulletMovement(EntityManager &entityManager, Screen& screen, EntityId entityId, float deltaTime)
{
	SystemManager& systemManager = SystemManager::GetInstance();
	Transform* transform = entityManager.GetComponent<Transform>(entityId);
	Velocity* velocity = entityManager.GetComponent<Velocity>(entityId);

	vec2 currentVelocity = velocity->GetVelocity();
	vec2 movement = currentVelocity * deltaTime;
	vec3 newPos = transform->GetPosition() + vec3(movement, 0.0f);
	transform->SetPosition(newPos);

	if (transform->GetPosition().x < screen.BORDER_LEFT_SCREEN_COORD || transform->GetPosition().x > screen.BORDER_RIGHT_SCREEN_COORD ||
		transform->GetPosition().y < screen.BORDER_TOP_SCREEN_COORD || transform->GetPosition().y > screen.BORDER_BOTTOM_SCREEN_COORD)
	{
		Event bulletOutOfBoundsEvent(EventType::BulletOutOfBounds, { entityId});
		systemManager.SendEvent(bulletOutOfBoundsEvent);
	}
}
