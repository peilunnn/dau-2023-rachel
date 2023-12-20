#pragma once
#include "Managers/include/EntityManager.h"
#include "Systems/include/System.h"

class AnimationHandler : public System
{
public:
    static AnimationHandler &GetInstance();
    AnimationHandler(AnimationHandler const &) = delete;
    void operator=(AnimationHandler const &) = delete;

    const float ENEMY_MELT_DURATION = 0.3f;
    const float LIGHTNING_FLASH_DURATION = 0.3f;
    const float PLAYER_DEATH_DURATION = 2.0f;

    const float ENEMY_ANIMATION_SPEED = 1.0f / 20.0f;
    const float HEALTH_BAR_ANIMATION_SPEED = 1.0f / 15.0f;
    const float LIGHTNING_STRIKE_ANIMATION_SPEED = 1.0f / 20.0f;
    const float LOADING_SCREEN_CHARACTER_ANIMATION_SPEED = 1.0f / 5.0f;
    const float PLAYER_ANIMATION_SPEED = 1.0f / 5.0f;
    const float ROTATION_SPEED = 0.8f;

    const float AMMO_PICKUP_MAX_SCALE = 0.2f;
    const float AMMO_PICKUP_MIN_SCALE = 0.15f;
    const float AMMO_PICKUP_SCALING_SPEED = 0.2f;
    const float HEALTH_PICKUP_MAX_SCALE = 2.75f;
    const float HEALTH_PICKUP_MIN_SCALE = 2.5f;
    const float HEALTH_PICKUP_SCALING_SPEED = 2.0f;
    const float LIGHTNING_PICKUP_MAX_SCALE = 0.175f;
    const float LIGHTNING_PICKUP_MIN_SCALE = 0.15f;
    const float LIGHTNING_PICKUP_SCALING_SPEED = 0.2f;

    const int HEALTH_BAR_MAX_FRAMES = 5;

    void Init();
    void Update(float deltaTime);
    void HandleEvent(const Event &event, float deltaTime) override;
    void RotatePlayer(float deltaTime);
    void SpinPickup(EntityId entityId, float deltaTime);
    void SpinAmmoPickup(EntityId entityId, float deltaTime);
    void SpinHealthPickup(EntityId entityId, float deltaTime);
    void SpinLightningPickup(EntityId entityId, float deltaTime);
    void ResetHealthBarAnimation();
    void ResetPlayerAnimation();
    void InitEnemyAnimation(SpriteManager& spriteManager, EntityId enemyEntityId);
    void PlayMeltAnimation(EntityId entityId, float deltaTime);
    void InitLightningStrikeAnimation(SpriteManager& spriteManager, EntityId enemyEntityId);

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
    void UpdateUIAnimation(EntityManager &entityManager, EntityId entityId, float deltaTime);
    void UpdateLightningStrikeAnimation(EntityManager &entityManager, EntityId entityId, float deltaTime);
    void HandleEnemyHitPlayer(EntityManager& entityManager, float deltaTime);
    void HandlePlayerHitHealthPickup();
    void SpinPickup(float deltaTime, EntityId pickupEntityId, float minScale, float maxScale, float scaleSpeed, bool& isScalingDown);
};