#pragma once
#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Animation.h"
#include "../../Components/include/Velocity.h"
#include "../../Components/include/Tag.h"
#include "../../Utilities/include/SimpleSprite.h"
#include "../../Systems/include/System.h"

class AnimationHandler : public System
{
public:
	int lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_FORWARDS;

	void Init(std::shared_ptr<CSimpleSprite> playerSprite, std::shared_ptr<CSimpleSprite> enemySprite, std::shared_ptr<CSimpleSprite> reloadingCircleSprite, std::shared_ptr<CSimpleSprite> healthBarSprite);
	void InitPlayerAnimation(std::shared_ptr<CSimpleSprite> playerSprite);
	static void InitEnemyAnimation(std::shared_ptr<CSimpleSprite> enemySprite);
	void InitReloadingCircleAnimation(std::shared_ptr<CSimpleSprite> reloadingCircleSprite);
	void InitHealthBarAnimation(std::shared_ptr<CSimpleSprite> healthBarSprite);
	void Update(EntityManager &entityManager, float deltaTime);
	void UpdatePlayerAnimation(EntityManager &entityManager, EntityId entityId, float deltaTime);
	void UpdateEnemyAnimation(EntityManager &entityManager, EntityId entityId, float deltaTime);
	void UpdateReloadingCircleAnimation(EntityManager &entityManager, EntityId entityId, float deltaTime);
	void UpdateHealthBarAnimation(EntityManager &entityManager, EntityId entityId, float deltaTime);
	void ProcessBulletHitEnemy(EntityManager &entityManager, EntityId entity1Id, EntityId entity2Id, float deltaTime);
	void ProcessEnemyHitPlayer(EntityManager &entityManager, float deltaTime);
	SystemType GetSystemType() const override
	{
		return SystemType::AnimationHandler;
	}
};