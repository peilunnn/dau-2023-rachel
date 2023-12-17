#include "stdafx.h"
#include "Components/include/Animation.h"
#include "Components/include/Health.h"
#include "Components/include/Pickup.h"
#include "Components/include/Renderable.h"
#include "Components/include/Tag.h"
#include "Components/include/Velocity.h"
#include "Managers/include/EntityManager.h"
#include "Managers/include/GameManager.h"
#include "Managers/include/SpriteManager.h"
#include "Systems/include/AnimationHandler.h"
#include "Systems/include/HealthHandler.h"
#include "Utilities/include/Enums.h"
#include "Utilities/include/Helper.h"
#include "Utilities/include/SimpleSprite.h"
using glm::vec2;

AnimationHandler &AnimationHandler::GetInstance()
{
	static AnimationHandler instance;
	return instance;
}

void AnimationHandler::Init()
{
	EntityManager &entityManager = EntityManager::GetInstance();
	SpriteManager &spriteManager = SpriteManager::GetInstance();

	InitPlayerAnimation(entityManager, spriteManager);
	InitHealthBarAnimation(entityManager, spriteManager);
	InitLoadingScreenCharacterAnimation(entityManager, spriteManager);
}

void AnimationHandler::InitPlayerAnimation(EntityManager &entityManager, SpriteManager &spriteManager)
{
	EntityId playerEntityId = entityManager.GetPlayerEntityId();
	CSimpleSprite *playerSprite = spriteManager.GetSprite(playerEntityId);

	constexpr float speed = 1.0f / 5.0f;
	playerSprite->CreateAnimation(PLAYER_ANIM_FORWARDS, speed, {12, 13, 14, 15});
	playerSprite->CreateAnimation(PLAYER_ANIM_BACKWARDS, speed, {0, 1, 2, 3});
	playerSprite->CreateAnimation(PLAYER_ANIM_LEFT, speed, {4, 5, 6, 7});
	playerSprite->CreateAnimation(PLAYER_ANIM_RIGHT, speed, {8, 9, 10, 11});
	playerSprite->CreateAnimation(PLAYER_ANIM_IDLE_BACKWARDS, speed, {0});
	playerSprite->CreateAnimation(PLAYER_ANIM_IDLE_LEFT, speed, {7});
	playerSprite->CreateAnimation(PLAYER_ANIM_IDLE_RIGHT, speed, {8});
	playerSprite->CreateAnimation(PLAYER_ANIM_IDLE_FORWARDS, speed, {12});
}

void AnimationHandler::InitEnemyAnimation(SpriteManager& spriteManager, EntityId enemyEntityId)
{
	CSimpleSprite* enemySprite = spriteManager.GetSprite(enemyEntityId);

	constexpr float speed = 1.0f / 20.0f;
	enemySprite->CreateAnimation(ENEMY_ANIM_IDLE, speed, { 0 });
	enemySprite->CreateAnimation(ENEMY_ANIM_MELT, speed, { 1,2,3,4,5,6,7 });
}

void AnimationHandler::InitHealthBarAnimation(EntityManager &entityManager, SpriteManager &spriteManager)
{
	EntityId healtBarEntityId = entityManager.GetHealthBarEntityId();
	CSimpleSprite *healthBarSprite = spriteManager.GetSprite(healtBarEntityId);

	constexpr float speed = 1.0f / 15.0f;
	healthBarSprite->CreateAnimation(HEALTH_100, speed, {0});
	healthBarSprite->CreateAnimation(HEALTH_80, speed, {2});
	healthBarSprite->CreateAnimation(HEALTH_60, speed, {4});
	healthBarSprite->CreateAnimation(HEALTH_40, speed, {1});
	healthBarSprite->CreateAnimation(HEALTH_20, speed, {3});
	healthBarSprite->CreateAnimation(HEALTH_0, speed, {5});
}

void AnimationHandler::InitLoadingScreenCharacterAnimation(EntityManager &entityManager, SpriteManager &spriteManager)
{
	EntityId loadingScreenCharacterEntityId = entityManager.GetLoadingScreenCharacterEntityId();
	CSimpleSprite *loadingScreenCharacterSprite = spriteManager.GetSprite(loadingScreenCharacterEntityId);

	constexpr float speed = 1.0f / 5.0f;
	loadingScreenCharacterSprite->CreateAnimation(PLAYER_ANIM_RIGHT, speed, {8, 9, 10, 11});
}

void AnimationHandler::Update(float deltaTime)
{
	GameManager &gameManager = GameManager::GetInstance();
	EntityManager &entityManager = EntityManager::GetInstance();

	if (gameManager.GetCurrentGameState() == GameState::Paused)
		return;

	for (auto entityId : entityManager.GetEntitiesWithComponents<Tag, Animation>())
	{
		Tag *tag = entityManager.GetComponent<Tag>(entityId);

		switch (tag->GetEntityType())
		{
		case EntityType::Player:
			UpdatePlayerAnimation(entityManager, entityId, deltaTime);
			break;
		case EntityType::Enemy:
			UpdateEnemyAnimation(entityManager, entityId, deltaTime);
			break;
		case EntityType::Pickup:
			SpinPickup(entityId, deltaTime);
			break;
		case EntityType::HealthBar:
			UpdateHealthBarAnimation(entityManager, entityId, deltaTime);
			break;
		case EntityType::LoadingScreenCharacter:
			UpdateLoadingScreenCharacterAnimation(entityManager, entityId, deltaTime);
			break;
		case EntityType::LightningStrike:
			UpdateLightningStrikeAnimation(entityManager, entityId, deltaTime);
			break;
		}
	}
}

void AnimationHandler::UpdatePlayerAnimation(EntityManager &entityManager, EntityId entityId, float deltaTime)
{
	Animation *animation = entityManager.GetComponent<Animation>(entityId);
	Velocity *velocity = entityManager.GetComponent<Velocity>(entityId);
	CSimpleSprite *sprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
	Tag *tag = entityManager.GetComponent<Tag>(entityId);

	if (!animation)
		return;

	vec2 currentVelocity = velocity->GetVelocity();
	if (dot(currentVelocity, currentVelocity) > 0)
	{
		if (currentVelocity.x > 0)
		{
			animation->SetCurrentAnimation(PLAYER_ANIM_RIGHT);
			m_lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_RIGHT;
		}
		else if (currentVelocity.x < 0)
		{
			animation->SetCurrentAnimation(PLAYER_ANIM_LEFT);
			m_lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_LEFT;
		}
		else if (currentVelocity.y > 0)
		{
			animation->SetCurrentAnimation(PLAYER_ANIM_FORWARDS);
			m_lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_FORWARDS;
		}
		else if (currentVelocity.y < 0)
		{
			animation->SetCurrentAnimation(PLAYER_ANIM_BACKWARDS);
			m_lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_BACKWARDS;
		}
	}
	else
		animation->SetCurrentAnimation(m_lastPlayerNonIdleAnimState);

	sprite->SetAnimation(animation->GetCurrentAnimation());
	sprite->Update(deltaTime);
}

void AnimationHandler::UpdateEnemyAnimation(EntityManager& entityManager, EntityId entityId, float deltaTime)
{
	CSimpleSprite* enemySprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
	enemySprite->Update(deltaTime);
}

void AnimationHandler::PlayMeltAnimation(EntityId entityId, float deltaTime)
{
	EntityManager& entityManager = EntityManager::GetInstance();

	CSimpleSprite* enemySprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
	enemySprite->SetAnimation(ENEMY_ANIM_MELT);
	enemySprite->Update(deltaTime);
}

void AnimationHandler::InitLightningStrikeAnimation(SpriteManager& spriteManager, EntityId lightningStrikeEntityId)
{
	CSimpleSprite* lightningStrikeSprite = spriteManager.GetSprite(lightningStrikeEntityId);

	constexpr float speed = 1.0f / 20.0f;
	lightningStrikeSprite->CreateAnimation(LIGHTNING_STRIKE_FLASH, speed, { 0, 1, 2, 3 });
}

void AnimationHandler::UpdateHealthBarAnimation(EntityManager &entityManager, EntityId entityId, float deltaTime)
{
	CSimpleSprite *healthBarSprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
	healthBarSprite->Update(deltaTime);
}

void AnimationHandler::UpdateLoadingScreenCharacterAnimation(EntityManager &entityManager, EntityId entityId, float deltaTime)
{
	CSimpleSprite *loadingScreenCharacterSprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
	Animation *loadingScreenCharacterAnimation = entityManager.GetComponent<Animation>(entityId);
	loadingScreenCharacterAnimation->SetCurrentAnimation(PLAYER_ANIM_RIGHT);
	loadingScreenCharacterSprite->SetAnimation(loadingScreenCharacterAnimation->GetCurrentAnimation());
	loadingScreenCharacterSprite->Update(deltaTime);
}

void AnimationHandler::UpdateLightningStrikeAnimation(EntityManager& entityManager, EntityId entityId, float deltaTime)
{
	CSimpleSprite* lightningStrikeSprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
	Animation* lightningStrikeAnimation = entityManager.GetComponent<Animation>(entityId);
	lightningStrikeAnimation->SetCurrentAnimation(LIGHTNING_STRIKE_FLASH);
	lightningStrikeSprite->SetAnimation(lightningStrikeAnimation->GetCurrentAnimation());
	lightningStrikeSprite->Update(deltaTime);
}

void AnimationHandler::HandleEvent(const Event &event, float deltaTime)
{
	EntityManager &entityManager = EntityManager::GetInstance();

	if (event.GetEventType() == EventType::EnemyHitPlayer)
		HandleEnemyHitPlayer(entityManager, deltaTime);
	else if (event.GetEventType() == EventType::PlayerHitHealthPickup)
		HandlePlayerHitHealthPickup();
}

void AnimationHandler::RotatePlayer(float deltaTime)
{
	EntityManager &entityManager = EntityManager::GetInstance();
	EntityId playerEntityId = entityManager.GetPlayerEntityId();
	CSimpleSprite *playerSprite = entityManager.GetComponent<Renderable>(playerEntityId)->GetSprite();
	
	constexpr float targetAngle = 67.5f;
	float currentAngle = playerSprite->GetAngle();
	const float rotationSpeed = 0.8f;
	float newAngle = currentAngle + rotationSpeed * deltaTime;
	playerSprite->SetAngle(newAngle);
}

void AnimationHandler::SpinPickup(EntityId entityId, float deltaTime)
{
	EntityManager& entityManager = EntityManager::GetInstance();
	Pickup* pickupComponent = entityManager.GetComponent<Pickup>(entityId);

	switch (pickupComponent->GetPickupType())
	{
		case PickupType::AmmoPickup:
			SpinAmmoPickup(entityId, deltaTime);
			break;
		case PickupType::HealthPickup:
			SpinHealthPickup(entityId, deltaTime);
			break;
		case PickupType::LightningPickup:
			SpinLightningPickup(entityId, deltaTime);
			break;
	}
}

void AnimationHandler::SpinAmmoPickup(EntityId entityId, float deltaTime)
{
	constexpr float minScale = 0.15f;
	constexpr float maxScale = 0.2f;
	constexpr float scaleSpeed = 0.2f;

	SpinPickup(deltaTime, entityId, minScale, maxScale, scaleSpeed, m_ammoPickupScalingDown);
}

void AnimationHandler::SpinHealthPickup(EntityId entityId, float deltaTime)
{
	constexpr float minScale = 2.5f;
	constexpr float maxScale = 2.75f;
	constexpr float scaleSpeed = 2.0f;

	SpinPickup(deltaTime, entityId, minScale, maxScale, scaleSpeed, m_healthPickupScalingDown);
}

void AnimationHandler::SpinLightningPickup(EntityId entityId, float deltaTime)
{
	constexpr float minScale = 0.15f;
	constexpr float maxScale = 0.175f;
	constexpr float scaleSpeed = 0.2f;

	SpinPickup(deltaTime, entityId, minScale, maxScale, scaleSpeed, m_lightningPickupScalingDown);
}

void AnimationHandler::ResetHealthBarAnimation()
{
	EntityManager &entityManager = EntityManager::GetInstance();
	EntityId healthBarEntityId = entityManager.GetHealthBarEntityId();
	CSimpleSprite *healthBarSprite = entityManager.GetComponent<Renderable>(healthBarEntityId)->GetSprite();
	Animation *healthBarAnimation = entityManager.GetComponent<Animation>(healthBarEntityId);
	healthBarAnimation->SetCurrentAnimation(HEALTH_100);
	healthBarSprite->SetAnimation(healthBarAnimation->GetCurrentAnimation());
}

void AnimationHandler::ResetPlayerAnimation()
{
	EntityManager &entityManager = EntityManager::GetInstance();
	EntityId playerEntityId = entityManager.GetPlayerEntityId();
	CSimpleSprite *playerSprite = entityManager.GetComponent<Renderable>(playerEntityId)->GetSprite();
	playerSprite->SetAngle(0.0f);
}

void AnimationHandler::HandleEnemyHitPlayer(EntityManager &entityManager, float deltaTime)
{
	EntityId playerEntityId = entityManager.GetPlayerEntityId();
	EntityId healthBarEntityId = entityManager.GetHealthBarEntityId();
	Health *health = entityManager.GetComponent<Health>(playerEntityId);
	Animation *animation = entityManager.GetComponent<Animation>(healthBarEntityId);
	CSimpleSprite *healthBarSprite = entityManager.GetComponent<Renderable>(healthBarEntityId)->GetSprite();
	constexpr int maxFrames = 5;

	if (!health || !healthBarSprite)
		return;

	int frameIndex = static_cast<int>((health->GetMaxHealth() - health->GetCurrentHealth()) / HealthHandler::GetInstance().GetHealthReduction());
	frameIndex = min(frameIndex, maxFrames);
	animation->SetCurrentAnimation(frameIndex);
	healthBarSprite->SetAnimation(animation->GetCurrentAnimation());
}

void AnimationHandler::HandlePlayerHitHealthPickup()
{
	ResetHealthBarAnimation();
}

void AnimationHandler::SpinPickup(float deltaTime, EntityId pickupEntityId, float minScale, float maxScale, float scaleSpeed, bool& isScalingDown)
{
	EntityManager& entityManager = EntityManager::GetInstance();

	Transform* pickupTransform = entityManager.GetComponent<Transform>(pickupEntityId);
	vec3 currentScale = pickupTransform->GetScale();

	if (isScalingDown)
	{
		currentScale.y -= scaleSpeed * deltaTime;
		if (currentScale.y <= minScale)
		{
			currentScale.y = minScale;
			isScalingDown = false;
		}
	}
	else
	{
		currentScale.y += scaleSpeed * deltaTime;
		if (currentScale.y >= maxScale)
		{
			currentScale.y = maxScale;
			isScalingDown = true;
		}
	}

	pickupTransform->SetScale(vec3(currentScale.x, currentScale.y, 1.0f));
}
