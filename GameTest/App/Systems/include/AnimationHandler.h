#ifndef ANIMATION_HANDLER_H
#define ANIMATION_HANDLER_H

#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Animation.h"
#include "../../Components/include/Velocity.h"
#include "../../Components/include/Tag.h"
#include "../../Utilities/include/SimpleSprite.h"
#include "../../Systems/include/System.h"

class AnimationHandler : public System
{
public:
	enum PlayerAnimationStates
	{
		PLAYER_ANIM_FORWARDS,
		PLAYER_ANIM_BACKWARDS,
		PLAYER_ANIM_LEFT,
		PLAYER_ANIM_RIGHT,
		PLAYER_ANIM_IDLE_LEFT,
		PLAYER_ANIM_IDLE_RIGHT,
		PLAYER_ANIM_IDLE_FORWARDS,
		PLAYER_ANIM_IDLE_BACKWARDS,
	};
	enum EnemyAnimationStates
	{
		ENEMY_ANIM_IDLE,
		ENEMY_ANIM_MELT,
	};
	enum ReloadingCircleAnimationStates
	{
		RELOADING_CIRCLE_ANIM_SPIN
	};
	enum HealthBarAnimationStates
	{
		HEALTH_100,
		HEALTH_80,
		HEALTH_60,
		HEALTH_40,
		HEALTH_20,
		HEALTH_0,
	};
	int lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_FORWARDS;

	void Init(std::shared_ptr<CSimpleSprite> playerSprite, std::shared_ptr<CSimpleSprite> enemySprite, std::shared_ptr<CSimpleSprite> reloadingCircleSprite, std::shared_ptr<CSimpleSprite> healthBarSprite);
	void InitPlayerAnimation(std::shared_ptr<CSimpleSprite> playerSprite);
	static void InitEnemyAnimation(std::shared_ptr<CSimpleSprite> enemySprite);
	void InitReloadingCircleAnimation(std::shared_ptr<CSimpleSprite> reloadingCircleSprite);
	void InitHealthBarAnimation(std::shared_ptr<CSimpleSprite> healthBarSprite);
	void Update(EntityManager& entityManager, float deltaTime);
	void UpdatePlayerAnimation(EntityManager& entityManager, Entity entity, float deltaTime);
	void UpdateEnemyAnimation(EntityManager& entityManager, Entity entity, float deltaTime);
	void UpdateReloadingCircleAnimation(EntityManager& entityManager, Entity entity, float deltaTime);
	void UpdateHealthBarAnimation(EntityManager& entityManager, Entity entity, float deltaTime);
	void ProcessBulletHitEnemy(EntityManager& entityManager, Entity entity1, Entity entity2, float deltaTime);
	void ProcessEnemyHitPlayer(EntityManager& entityManager, float deltaTime);
	Type GetSystemType() const override {
		return System::Type::AnimationHandler;
	}
};

#endif // ANIMATION_HANDLER_H