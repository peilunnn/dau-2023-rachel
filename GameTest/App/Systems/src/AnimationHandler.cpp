#include "stdafx.h"
#include "Components/include/Animation.h"
#include "Components/include/Cooldown.h"
#include "Components/include/Health.h"
#include "Components/include/Renderable.h"
#include "Components/include/Tag.h"
#include "Components/include/Velocity.h"
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"
#include "Systems/include/MovementHandler.h"
#include "Systems/include/HealthHandler.h"
#include "Utilities/include/SimpleSprite.h"
#include "Utilities/include/Enums.h"
#include "Utilities/include/Helper.h"
#include "../include/AnimationHandler.h"
using glm::vec2;

void AnimationHandler::Init(shared_ptr<CSimpleSprite> playerSprite, shared_ptr<CSimpleSprite> enemySprite, shared_ptr<CSimpleSprite> reloadingCircleSprite, shared_ptr<CSimpleSprite> healthBarSprite)
{
	InitPlayerAnimation(playerSprite);
	InitReloadingCircleAnimation(reloadingCircleSprite);
	InitHealthBarAnimation(healthBarSprite);
}

void AnimationHandler::InitPlayerAnimation(shared_ptr<CSimpleSprite> playerSprite)
{
	if (!playerSprite)
		return;

	float speed = 1.0f / 15.0f;
	playerSprite->CreateAnimation(PLAYER_ANIM_FORWARDS, speed, {24, 25, 26, 27, 28, 29, 30, 31});
	playerSprite->CreateAnimation(PLAYER_ANIM_BACKWARDS, speed, {0, 1, 2, 3, 4, 5, 6, 7});
	playerSprite->CreateAnimation(PLAYER_ANIM_LEFT, speed, {8, 9, 10, 11, 12, 13, 14, 15});
	playerSprite->CreateAnimation(PLAYER_ANIM_RIGHT, speed, {16, 17, 18, 19, 20, 21, 22, 23});
	playerSprite->CreateAnimation(PLAYER_ANIM_IDLE_BACKWARDS, speed, {0});
	playerSprite->CreateAnimation(PLAYER_ANIM_IDLE_LEFT, speed, {8});
	playerSprite->CreateAnimation(PLAYER_ANIM_IDLE_RIGHT, speed, {16});
	playerSprite->CreateAnimation(PLAYER_ANIM_IDLE_FORWARDS, speed, {24});
}

void AnimationHandler::HandleEvent(const Event& event, float deltaTime)
{
	EntityManager& entityManager = EntityManager::GetInstance();

	if (event.GetEventType() == "EnemyHitPlayer") {
		HandleEnemyHitPlayer(entityManager, deltaTime);
	}
}

void AnimationHandler::InitReloadingCircleAnimation(shared_ptr<CSimpleSprite> reloadingCircleSprite)
{
	if (!reloadingCircleSprite)
		return;

	float speed = 1.0f / 15.0f;
	reloadingCircleSprite->CreateAnimation(RELOADING_CIRCLE_ANIM_SPIN, speed, {1, 2, 3, 4, 5, 6});
}

void AnimationHandler::InitHealthBarAnimation(shared_ptr<CSimpleSprite> healthBarSprite)
{
	if (!healthBarSprite)
		return;

	float speed = 1.0f / 15.0f;
	healthBarSprite->CreateAnimation(HEALTH_100, speed, {0});
	healthBarSprite->CreateAnimation(HEALTH_80, speed, {2});
	healthBarSprite->CreateAnimation(HEALTH_60, speed, {4});
	healthBarSprite->CreateAnimation(HEALTH_40, speed, {1});
	healthBarSprite->CreateAnimation(HEALTH_20, speed, {3});
	healthBarSprite->CreateAnimation(HEALTH_0, speed, {5});
}

void AnimationHandler::Update(float deltaTime)
{
	EntityManager& entityManager = EntityManager::GetInstance();

	for (auto entityId : entityManager.GetEntitiesWithComponents<Tag, Animation>())
	{
		Tag* tag = entityManager.GetComponent<Tag>(entityId);

		switch (tag->GetEntityType())
		{
		case EntityType::Player:
			UpdatePlayerAnimation(entityManager, entityId, deltaTime);
			break;
		case EntityType::ReloadingCircle:
			UpdateReloadingCircleAnimation(entityManager, entityId, deltaTime);
			break;
		case EntityType::HealthBar:
			UpdateHealthBarAnimation(entityManager, entityId, deltaTime);
			break;
		}
	}
}

void AnimationHandler::UpdatePlayerAnimation(EntityManager &entityManager, EntityId entityId, float deltaTime)
{
	Animation* animation = entityManager.GetComponent<Animation>(entityId);
	Velocity* velocity = entityManager.GetComponent<Velocity>(entityId);
	shared_ptr<CSimpleSprite> sprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
	Tag* tag = entityManager.GetComponent<Tag>(entityId);

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

void AnimationHandler::UpdateReloadingCircleAnimation(EntityManager &entityManager, EntityId entityId, float deltaTime)
{
	shared_ptr<CSimpleSprite> reloadingCircleSprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
	Animation* reloadingCircleAnimation = entityManager.GetComponent<Animation>(entityId);
	reloadingCircleAnimation->SetCurrentAnimation(RELOADING_CIRCLE_ANIM_SPIN);
	reloadingCircleSprite->SetAnimation(RELOADING_CIRCLE_ANIM_SPIN);
	reloadingCircleSprite->Update(deltaTime);
}

void AnimationHandler::UpdateHealthBarAnimation(EntityManager &entityManager, EntityId entityId, float deltaTime)
{
	shared_ptr<CSimpleSprite> healthBarSprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
	healthBarSprite->Update(deltaTime);
}

void AnimationHandler::HandleEnemyHitPlayer(EntityManager &entityManager, float deltaTime)
{
	EntityId playerEntityId = entityManager.GetPlayerEntityId();
	EntityId healthBarEntityId = entityManager.GetHealthBarEntityId();
	Health* health = entityManager.GetComponent<Health>(playerEntityId);
	Animation* animation = entityManager.GetComponent<Animation>(healthBarEntityId);
	shared_ptr<CSimpleSprite> healthBarSprite = entityManager.GetComponent<Renderable>(healthBarEntityId)->GetSprite();
	constexpr int maxFrames = 5;

	if (!health || !healthBarSprite)
		return;

	int frameIndex = static_cast<int>((health->GetMaxHealth() - health->GetCurrentHealth()) / HealthHandler::GetInstance().GetHealthReduction());
	frameIndex = min(frameIndex, maxFrames);
	animation->SetCurrentAnimation(frameIndex);
	healthBarSprite->SetAnimation(frameIndex);
}
