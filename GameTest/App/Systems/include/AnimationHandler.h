#ifndef HANDLE_ANIMATION_H
#define HANDLE_ANIMATION_H

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
	int lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_FORWARDS;

	void InitPlayerAnimation(std::shared_ptr<CSimpleSprite> sprite);
	static void InitEnemyAnimation(std::shared_ptr<CSimpleSprite> sprite);
	void Update(EntityManager& entityManager, float deltaTime);
	void UpdatePlayerAnimation(EntityManager& entityManager, Entity entity, float deltaTime);
	void ProcessBulletHitEnemy(EntityManager& entityManager, Entity entity1, Entity entity2, float deltaTime);
	Type GetSystemType() const override {
		return System::Type::AnimationHandler;
	}
};

#endif // HANDLE_ANIMATION_H