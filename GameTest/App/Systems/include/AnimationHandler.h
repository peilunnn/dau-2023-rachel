#pragma once
#include "Utilities/include/SimpleSprite.h"
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"
using namespace std;

class AnimationHandler : public System
{
public:
	int lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_FORWARDS;

	void Init(shared_ptr<CSimpleSprite> playerSprite, shared_ptr<CSimpleSprite> enemySprite, shared_ptr<CSimpleSprite> reloadingCircleSprite, shared_ptr<CSimpleSprite> healthBarSprite);
	void InitPlayerAnimation(shared_ptr<CSimpleSprite> playerSprite);
	static void InitEnemyAnimation(shared_ptr<CSimpleSprite> enemySprite);
	void InitReloadingCircleAnimation(shared_ptr<CSimpleSprite> reloadingCircleSprite);
	void InitHealthBarAnimation(shared_ptr<CSimpleSprite> healthBarSprite);
	void Update(EntityManager& entityManager, float deltaTime);
	void UpdatePlayerAnimation(EntityManager& entityManager, EntityId entityId, float deltaTime);
	void UpdateEnemyAnimation(EntityManager& entityManager, EntityId entityId, float deltaTime);
	void UpdateReloadingCircleAnimation(EntityManager& entityManager, EntityId entityId, float deltaTime);
	void UpdateHealthBarAnimation(EntityManager& entityManager, EntityId entityId, float deltaTime);
	void ProcessBulletHitEnemy(EntityManager& entityManager, EntityId entity1Id, EntityId entity2Id, float deltaTime);
	void ProcessEnemyHitPlayer(EntityManager& entityManager, float deltaTime);
	SystemType GetSystemType() const override
	{
		return SystemType::AnimationHandler;
	}
};