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
	void UpdatePlayerAnimation(EntityManager &entityManager, Entity entity, float deltaTime);
	void UpdateEnemyAnimation(EntityManager &entityManager, Entity entity, float deltaTime);
	void UpdateReloadingCircleAnimation(EntityManager &entityManager, Entity entity, float deltaTime);
	void UpdateHealthBarAnimation(EntityManager &entityManager, Entity entity, float deltaTime);
	void ProcessBulletHitEnemy(EntityManager &entityManager, Entity entity1, Entity entity2, float deltaTime);
	void ProcessEnemyHitPlayer(EntityManager &entityManager, float deltaTime);
	SystemType GetSystemType() const override
	{
		return SystemType::AnimationHandler;
	}
};