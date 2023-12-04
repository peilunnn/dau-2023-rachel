#include "stdafx.h"
#include "Components/include/Animation.h"
#include "Components/include/Tag.h"
#include "Components/include/Velocity.h"
#include "Components/include/Health.h"
#include "Components/include/Renderable.h"
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"
#include "Utilities/include/SimpleSprite.h"
#include "Utilities/include/Enums.h"
#include "../include/AnimationHandler.h"
using glm::vec2;

void AnimationHandler::Init(shared_ptr<CSimpleSprite> playerSprite, shared_ptr<CSimpleSprite> enemySprite, shared_ptr<CSimpleSprite> reloadingCircleSprite, shared_ptr<CSimpleSprite> healthBarSprite)
{
	InitPlayerAnimation(playerSprite);
	InitEnemyAnimation(enemySprite);
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

void AnimationHandler::InitEnemyAnimation(shared_ptr<CSimpleSprite> enemySprite)
{
	if (!enemySprite)
		return;

	float speed = 1.0f / 15.0f;
	enemySprite->CreateAnimation(ENEMY_ANIM_IDLE, speed, {0});
	enemySprite->CreateAnimation(ENEMY_ANIM_MELT, speed, {1, 2, 3, 4, 5, 6, 7});
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

void AnimationHandler::Update(EntityManager &entityManager, float deltaTime)
{
	for (auto entityId : entityManager.GetEntitiesWithComponents<Tag, Animation>())
	{
		shared_ptr<Tag> tag = entityManager.GetComponent<Tag>(entityId);

		switch (tag->GetEntityType())
		{
		case EntityType::Player:
			UpdatePlayerAnimation(entityManager, entityId, deltaTime);
			break;
		case EntityType::Enemy:
			UpdateEnemyAnimation(entityManager, entityId, deltaTime);
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
	shared_ptr<Animation> animation = entityManager.GetComponent<Animation>(entityId);
	vec2 velocity = entityManager.GetComponent<Velocity>(entityId)->GetVelocity();
	shared_ptr<CSimpleSprite> sprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
	shared_ptr<Tag> tag = entityManager.GetComponent<Tag>(entityId);

	if (!animation)
		return;

	if (dot(velocity, velocity) > 0)
	{
		if (velocity.x > 0)
		{
			animation->SetCurrentAnimation(PLAYER_ANIM_RIGHT);
			m_lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_RIGHT;
		}
		else if (velocity.x < 0)
		{
			animation->SetCurrentAnimation(PLAYER_ANIM_LEFT);
			m_lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_LEFT;
		}
		else if (velocity.y > 0)
		{
			animation->SetCurrentAnimation(PLAYER_ANIM_FORWARDS);
			m_lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_FORWARDS;
		}
		else if (velocity.y < 0)
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

void AnimationHandler::UpdateEnemyAnimation(EntityManager &entityManager, EntityId entityId, float deltaTime)
{
	shared_ptr<CSimpleSprite> enemySprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
	shared_ptr<Animation> enemyAnimation = entityManager.GetComponent<Animation>(entityId);
	enemySprite->Update(deltaTime);

	if (enemyAnimation->GetCooldownTimer() > 0.0f)
	{
		enemyAnimation->SetCooldownTimer(enemyAnimation->GetCooldownTimer() - deltaTime);
		if (enemyAnimation->GetCooldownTimer() <= 0.0f)
			entityManager.MarkEntityForDeletion(entityId);
	}
}

void AnimationHandler::UpdateReloadingCircleAnimation(EntityManager &entityManager, EntityId entityId, float deltaTime)
{
	shared_ptr<CSimpleSprite> reloadingCircleSprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
	shared_ptr<Animation> reloadingCircleAnimation = entityManager.GetComponent<Animation>(entityId);
	reloadingCircleAnimation->SetCurrentAnimation(RELOADING_CIRCLE_ANIM_SPIN);
	reloadingCircleSprite->SetAnimation(RELOADING_CIRCLE_ANIM_SPIN);
	reloadingCircleSprite->Update(deltaTime);
}

void AnimationHandler::UpdateHealthBarAnimation(EntityManager &entityManager, EntityId entityId, float deltaTime)
{
	shared_ptr<CSimpleSprite> healthBarSprite = entityManager.GetComponent<Renderable>(entityId)->GetSprite();
	healthBarSprite->Update(deltaTime);
}

void AnimationHandler::HandleBulletHitEnemy(EntityManager &entityManager, EntityId firstEntityId, EntityId secondEntityId, float deltaTime)
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

	shared_ptr<Animation> enemyAnimation = entityManager.GetComponent<Animation>(enemyEntityId);
	shared_ptr<Velocity> enemyVelocity = entityManager.GetComponent<Velocity>(enemyEntityId);
	shared_ptr<CSimpleSprite> enemySprite = entityManager.GetComponent<Renderable>(enemyEntityId)->GetSprite();

	if (!enemyAnimation || !enemyVelocity || !enemySprite)
		return;

	enemyAnimation->SetCurrentAnimation(ENEMY_ANIM_MELT);
	enemySprite->SetAnimation(ENEMY_ANIM_MELT);
	enemyVelocity->SetVelocity(glm::vec2(0.0f, 0.0f));

	if (enemySprite->IsAnimationComplete())
		enemyAnimation->SetCooldownTimer(0.3f);

	entityManager.MarkEntityForDeletion(bulletEntityId);
}

void AnimationHandler::HandleEnemyHitPlayer(EntityManager &entityManager, float deltaTime)
{
	EntityId playerEntityId = entityManager.GetPlayerEntityId();
	EntityId healthBarEntityId = entityManager.GetHealthBarEntityId();
	shared_ptr<Health> health = entityManager.GetComponent<Health>(playerEntityId);
	shared_ptr<Animation> animation = entityManager.GetComponent<Animation>(healthBarEntityId);
	shared_ptr<CSimpleSprite> healthBarSprite = entityManager.GetComponent<Renderable>(healthBarEntityId)->GetSprite();

	if (!health || !healthBarSprite)
		return;

	int frameIndex = static_cast<int>((health->GetMaxHealth() - health->GetCurrentHealth()) / 20);
	frameIndex = min(frameIndex, 5);
	animation->SetCurrentAnimation(frameIndex);
	healthBarSprite->SetAnimation(frameIndex);
}
