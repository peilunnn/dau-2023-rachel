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
using std::min;

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

	playerSprite->CreateAnimation(PLAYER_ANIM_FORWARDS, PLAYER_ANIMATION_SPEED, {12, 13, 14, 15});
	playerSprite->CreateAnimation(PLAYER_ANIM_BACKWARDS, PLAYER_ANIMATION_SPEED, {0, 1, 2, 3});
	playerSprite->CreateAnimation(PLAYER_ANIM_LEFT, PLAYER_ANIMATION_SPEED, {4, 5, 6, 7});
	playerSprite->CreateAnimation(PLAYER_ANIM_RIGHT, PLAYER_ANIMATION_SPEED, {8, 9, 10, 11});
	playerSprite->CreateAnimation(PLAYER_ANIM_IDLE_BACKWARDS, PLAYER_ANIMATION_SPEED, {0});
	playerSprite->CreateAnimation(PLAYER_ANIM_IDLE_LEFT, PLAYER_ANIMATION_SPEED, {7});
	playerSprite->CreateAnimation(PLAYER_ANIM_IDLE_RIGHT, PLAYER_ANIMATION_SPEED, {8});
	playerSprite->CreateAnimation(PLAYER_ANIM_IDLE_FORWARDS, PLAYER_ANIMATION_SPEED, {12});
}

void AnimationHandler::InitEnemyAnimation(SpriteManager& spriteManager, EntityId enemyEntityId)
{
	CSimpleSprite* enemySprite = spriteManager.GetSprite(enemyEntityId);

	enemySprite->CreateAnimation(ENEMY_ANIM_IDLE, ENEMY_ANIMATION_SPEED, { 0 });
	enemySprite->CreateAnimation(ENEMY_ANIM_MELT, ENEMY_ANIMATION_SPEED, { 1,2,3,4,5,6,7 });
}

void AnimationHandler::InitHealthBarAnimation(EntityManager &entityManager, SpriteManager &spriteManager)
{
	EntityId healtBarEntityId = entityManager.GetHealthBarEntityId();
	CSimpleSprite *healthBarSprite = spriteManager.GetSprite(healtBarEntityId);

	healthBarSprite->CreateAnimation(HEALTH_100, HEALTH_BAR_ANIMATION_SPEED, {0});
	healthBarSprite->CreateAnimation(HEALTH_80, HEALTH_BAR_ANIMATION_SPEED, {2});
	healthBarSprite->CreateAnimation(HEALTH_60, HEALTH_BAR_ANIMATION_SPEED, {4});
	healthBarSprite->CreateAnimation(HEALTH_40, HEALTH_BAR_ANIMATION_SPEED, {1});
	healthBarSprite->CreateAnimation(HEALTH_20, HEALTH_BAR_ANIMATION_SPEED, {3});
	healthBarSprite->CreateAnimation(HEALTH_0, HEALTH_BAR_ANIMATION_SPEED, {5});
}

void AnimationHandler::InitLoadingScreenCharacterAnimation(EntityManager &entityManager, SpriteManager &spriteManager)
{
	EntityId entityId = entityManager.GetLoadingScreenCharacterEntityId();
	CSimpleSprite *sprite = spriteManager.GetSprite(entityId);

	sprite->CreateAnimation(PLAYER_ANIM_RIGHT, LOADING_SCREEN_CHARACTER_ANIMATION_SPEED, {8, 9, 10, 11});
	sprite->SetAnimation(PLAYER_ANIM_RIGHT);
}

void AnimationHandler::Update(float deltaTime)
{
	GameManager &gameManager = GameManager::GetInstance();
	EntityManager &entityManager = EntityManager::GetInstance();

	if (gameManager.GetCurrentGameState() == GameState::Paused)
		return;

	for (EntityId entityId : entityManager.GetEntitiesWithComponents<Tag, Animation>())
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
		case EntityType::UI:
			UpdateUIAnimation(entityManager, entityId, deltaTime);
			break;
		case EntityType::LightningStrike:
			UpdateLightningStrikeAnimation(entityManager, entityId, deltaTime);
			break;
		}
	}
}

void AnimationHandler::UpdatePlayerAnimation(EntityManager &entityManager, EntityId entityId, float deltaTime)
{
	Velocity *velocity = entityManager.GetComponent<Velocity>(entityId);
	CSimpleSprite *sprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
	Tag *tag = entityManager.GetComponent<Tag>(entityId);

	vec2 currentVelocity = velocity->GetVelocity();
	if (dot(currentVelocity, currentVelocity) > 0)
	{
		if (currentVelocity.x > 0)
		{
			sprite->SetAnimation(PLAYER_ANIM_RIGHT);
			m_lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_RIGHT;
		}
		else if (currentVelocity.x < 0)
		{
			sprite->SetAnimation(PLAYER_ANIM_LEFT);
			m_lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_LEFT;
		}
		else if (currentVelocity.y > 0)
		{
			sprite->SetAnimation(PLAYER_ANIM_FORWARDS);
			m_lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_FORWARDS;
		}
		else if (currentVelocity.y < 0)
		{
			sprite->SetAnimation(PLAYER_ANIM_BACKWARDS);
			m_lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_BACKWARDS;
		}
	}
	else
		sprite->SetAnimation(m_lastPlayerNonIdleAnimState);

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

	lightningStrikeSprite->CreateAnimation(LIGHTNING_STRIKE_FLASH, LIGHTNING_STRIKE_ANIMATION_SPEED, { 0, 1, 2, 3 });
}

void AnimationHandler::UpdateUIAnimation(EntityManager &entityManager, EntityId entityId, float deltaTime)
{
	CSimpleSprite *sprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
	sprite->Update(deltaTime);
}

void AnimationHandler::UpdateLightningStrikeAnimation(EntityManager& entityManager, EntityId entityId, float deltaTime)
{
	CSimpleSprite* lightningStrikeSprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
	lightningStrikeSprite->SetAnimation(LIGHTNING_STRIKE_FLASH);
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
	
	float currentAngle = playerSprite->GetAngle();
	float newAngle = currentAngle + ROTATION_SPEED * deltaTime;
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
	SpinPickup(deltaTime, entityId, AMMO_PICKUP_MIN_SCALE, AMMO_PICKUP_MAX_SCALE, AMMO_PICKUP_SCALING_SPEED, m_ammoPickupScalingDown);
}

void AnimationHandler::SpinHealthPickup(EntityId entityId, float deltaTime) 
{
	SpinPickup(deltaTime, entityId, HEALTH_PICKUP_MIN_SCALE, HEALTH_PICKUP_MAX_SCALE, HEALTH_PICKUP_SCALING_SPEED, m_healthPickupScalingDown);
}

void AnimationHandler::SpinLightningPickup(EntityId entityId, float deltaTime) 
{
	SpinPickup(deltaTime, entityId, LIGHTNING_PICKUP_MIN_SCALE, LIGHTNING_PICKUP_MAX_SCALE, LIGHTNING_PICKUP_SCALING_SPEED, m_lightningPickupScalingDown);
}

void AnimationHandler::ResetHealthBarAnimation()
{
	EntityManager &entityManager = EntityManager::GetInstance();
	EntityId healthBarEntityId = entityManager.GetHealthBarEntityId();
	CSimpleSprite *healthBarSprite = entityManager.GetComponent<Renderable>(healthBarEntityId)->GetSprite();
	healthBarSprite->SetAnimation(HEALTH_100);
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
	CSimpleSprite *healthBarSprite = entityManager.GetComponent<Renderable>(healthBarEntityId)->GetSprite();

	if (!health || !healthBarSprite)
		return;

	int frameIndex = static_cast<int>((health->GetMaxHealth() - health->GetCurrentHealth()) / HealthHandler::GetInstance().GetHealthReduction());
	frameIndex = min(frameIndex, HEALTH_BAR_MAX_FRAMES);
	healthBarSprite->SetAnimation(frameIndex);
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
