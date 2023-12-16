#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"
#include "Utilities/include/SimpleSprite.h"
#include <set>
#include <string>
using namespace std;

class AnimationHandler : public System
{
public:
    static AnimationHandler &GetInstance();
    AnimationHandler(AnimationHandler const &) = delete;
    void operator=(AnimationHandler const &) = delete;

    void Init();
    void Update(float deltaTime);
    void HandleEvent(const Event &event, float deltaTime) override;
    void RotatePlayer(float deltaTime);
    void SpinAmmoPickup(float deltaTime);
    void SpinHealthPickup(float deltaTime);
    void SpinLightningPickup(float deltaTime);
    void ResetHealthBarAnimation();
    void ResetPlayerAnimation();
    void InitEnemyAnimation(EntityManager& entityManager, SpriteManager& spriteManager, EntityId enemyEntityId);
    void PlayMeltAnimation(EntityId entityId, float deltaTime);

private:
    AnimationHandler()
    {
        m_subscribedEvents.insert(EventType::EnemyHitPlayer);
        m_subscribedEvents.insert(EventType::PlayerHitHealthPickup);
    }

    int m_lastPlayerNonIdleAnimState = PLAYER_ANIM_IDLE_FORWARDS;
    bool m_ammoPickupScalingDown = true;
    bool m_healthPickupScalingDown = true;
    bool m_lightningPickupScalingDown = true;

    void InitPlayerAnimation(EntityManager& entityManager, SpriteManager& spriteManager);
    void InitHealthBarAnimation(EntityManager &entityManager, SpriteManager &spriteManager);
    void InitLoadingScreenCharacterAnimation(EntityManager &entityManager, SpriteManager &spriteManager);
    void UpdatePlayerAnimation(EntityManager& entityManager, EntityId entityId, float deltaTime);
    void UpdateEnemyAnimation(EntityManager& entityManager, EntityId entityId, float deltaTime);
    void UpdateHealthBarAnimation(EntityManager &entityManager, EntityId entityId, float deltaTime);
    void UpdateLoadingScreenCharacterAnimation(EntityManager &entityManager, EntityId entityId, float deltaTime);
    void HandleEnemyHitPlayer(EntityManager& entityManager, float deltaTime);
    void HandlePlayerHitHealthPickup();
    void SpinPickup(float deltaTime, EntityId pickupEntityId, float minScale, float maxScale, float scaleSpeed, bool& isScalingDown);
};