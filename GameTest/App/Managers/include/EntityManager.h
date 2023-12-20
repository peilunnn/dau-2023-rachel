#pragma once
#include "Components/include/Component.h"
#include "Utilities/include/EntityId.h"
#include "Managers/include/SpriteManager.h"
#include "Utilities/include/Enums.h"
#include <glm/fwd.hpp>
#include <memory>
#include <unordered_map>
#include <vector>
#include <glm/glm.hpp>
using glm::vec2;
using glm::vec3;
using std::vector;
using std::move;
using std::unique_ptr;

class EntityManager
{
public:
    static EntityManager &GetInstance();
    EntityManager(EntityManager const &) = delete;
    void operator=(EntityManager const &) = delete;

    void Init();
    vector<EntityId> GetAllEntityIds();
    void ResetEnemies();
    void SetEntityStateAndVisibility(EntityId entityId, EntityState state, bool isVisible);
    EntityId CreateLightningStrikeEntity(SpriteManager& spriteManager);
    EntityId CreateParticleEntity(SpriteManager& spriteManager, ParticleType particleType);

    EntityId GetBulletFromPool();
    void ReturnBulletToPool(EntityId bulletEntityId);
    EntityId GetEnemyFromPool();
    void ReturnEnemyToPool(EntityId enemyEntityId);
    EntityId GetLightningStrikeFromPool();
    void ReturnLightningStrikeToPool(EntityId lightningStrikeEntityId);

    EntityId GetPlayerEntityId() const { return m_playerEntityId; }
    EntityId GetAmmoPickupEntityId() const { return m_ammoPickupEntityId; }
    EntityId GetHealthPickupEntityId() const { return m_healthPickupEntityId; }
    EntityId GetLightningPickupEntityId() const { return m_lightningPickupEntityId; }
    EntityId GetHealthBarEntityId() const { return m_healthBarEntityId; }
    EntityId GetScoreEntityId() const { return m_scoreEntityId; }
    EntityId GetCountdownTimerEntityId() const { return m_countdownTimerEntityId; }
    EntityId GetTitleEntityId() const { return m_titleEntityId; }
    EntityId GetPlayButtonEntityId() const { return m_playButtonEntityId; }
    EntityId GetBackButtonEntityId() const { return m_backButtonEntityId; }
    EntityId GetQuitButtonEntityId() const { return m_quitButtonEntityId; }
    EntityId GetLoadingScreenCharacterEntityId() const { return m_loadingScreenCharacterEntityId; }
    EntityId GetStarfieldEntityId() const { return m_starfieldEntityId; }
    EntityId GetCrosshairEntityId() const { return m_crosshairEntityId; }
    EntityId GetLightningStrikeEntityId() const { return m_lightningStrikeEntityId; }
    vector<EntityId> GetAmmoEmptyEntityIds() const { return m_ammoEmptyEntityIds; }
    vector<EntityId> GetAmmoFilledEntityIds() const { return m_ammoFilledEntityIds; }

    template <typename T>
    void AddComponent(EntityId entityId, unique_ptr<T> component)
    {
        m_entityComponents[entityId].push_back(move(component));
    }

    template <typename T>
    T *GetComponent(EntityId entityId)
    {
        auto it = m_entityComponents.find(entityId);
        if (it != m_entityComponents.end())
        {
            for (auto &comp : it->second)
            {
                if (auto casted = dynamic_cast<T *>(comp.get()))
                {
                    return casted;
                }
            }
        }
        return nullptr;
    }

    template <typename... Components>
    vector<EntityId> GetEntitiesWithComponents()
    {
        vector<EntityId> entitiesWithComponents;
        for (const auto &pair : m_entityComponents)
        {
            bool hasAllComponents = ((GetComponent<Components>(pair.first) != nullptr) && ...);
            if (hasAllComponents)
                entitiesWithComponents.push_back(pair.first);
        }
        return entitiesWithComponents;
    }

private:
    EntityManager() = default;

    const int AMMO_SPRITE_SPACING = 30;
    const float PLAYER_SHOOTING_COOLDOWN = 0.5f;

    const float AMMO_PICKUP_RADIUS_MULTIPLIER = 0.15f;
    const float BULLET_RADIUS_MULTIPLIER = 0.25f;
    const float ENEMY_RADIUS_MULTIPLIER = 0.25f;
    const float LIGHTNING_PICKUP_RADIUS_MULTIPLIER = 0.15f;
    const float PLAYER_RADIUS_MULTIPLIER = 0.25f;
    
    const float ZERO_POS_FLOAT = 0.0f;
    const vec2 ZERO_VELOCITY = vec2(0.0f);
    const vec3 ZERO_POS_VEC3 = vec3(0.0f);
    const vec3 ZERO_ROT = vec3(0.0f);
    
    const vec3 AMMO_PICKUP_SCALE = vec3(0.15f);
    const vec3 AMMO_SCALE = vec3(0.5f);
    const vec3 BULLET_SCALE = vec3(1.0f);
    const vec3 BUTTON_SCALE = vec3(0.2f);
    const vec3 COUNTDOWN_TIMER_SCALE = vec3(1.0f);
    const vec3 CROSSHAIR_SCALE = vec3(1.5f);
    const vec3 DUST_PARTICLE_SCALE = vec3(1.0f);
    const vec3 ENEMY_SCALE = vec3(0.4f);
    const vec3 HEALTH_BAR_SCALE = vec3(1.0f);
    const vec3 HEALTH_PICKUP_SCALE = vec3(2.0f);
    const vec3 LIGHTNING_PICKUP_SCALE = vec3(0.15f);
    const vec3 LIGHTNING_STRIKE_SCALE = vec3(1.0f);
    const vec3 LOADING_SCREEN_CHARACTER_SCALE = vec3(0.5f);
    const vec3 PLAYER_SCALE = vec3(0.6f);
    const vec3 SCORE_SCALE = vec3(1.0f);
    const vec3 STARFIELD_SCALE = vec3(2.0f);
    const vec3 TITLE_SCALE = vec3(0.5f);

    const float AMMO_X_OFFSET = 20.0f;
    const float AMMO_Y_OFFSET = 720.0f;
    const float BUTTON_X_OFFSET = 520.0f;
    const float BUTTON_Y_OFFSET = 400.0f;
    const float COUNTDOWN_TIMER_X_OFFSET = 1000.0f;
    const float COUNTDOWN_TIMER_Y_OFFSET = 50.0f;
    const float HEALTH_BAR_X_OFFSET = 880.0f;
    const float HEALTH_BAR_Y_OFFSET = 720.0f;
    const float LOADING_SCREEN_CHARACTER_X_OFFSET = 520.0f;
    const float LOADING_SCREEN_CHARACTER_Y_OFFSET = 340.0f;
    const float SCORE_X_OFFSET = 100.0f;
    const float SCORE_Y_OFFSET = 50.0f;
    const float TITLE_X_OFFSET = 520.0f;
    const float TITLE_Y_OFFSET = 200.0f;

    EntityId m_nextEntityId = -1;
    EntityId m_playerEntityId = -1;
    EntityId m_enemyEntityId = -1;
    EntityId m_ammoPickupEntityId = -1;
    EntityId m_healthPickupEntityId = -1;
    EntityId m_lightningPickupEntityId = -1;
    EntityId m_ammoEmptyEntityId = -1;
    EntityId m_ammoFilledEntityId = -1;
    EntityId m_healthBarEntityId = -1;
    EntityId m_scoreEntityId = -1;
    EntityId m_countdownTimerEntityId = -1;
    EntityId m_titleEntityId = -1;
    EntityId m_playButtonEntityId = -1;
    EntityId m_backButtonEntityId = -1;
    EntityId m_quitButtonEntityId = -1;
    EntityId m_loadingScreenCharacterEntityId = -1;
    EntityId m_starfieldEntityId = -1;
    EntityId m_crosshairEntityId = -1;
    EntityId m_lightningStrikeEntityId = -1;
    unordered_map<EntityId, vector<unique_ptr<Component>>> m_entityComponents = {};
    vector<EntityId> m_entitiesToDelete = vector<EntityId>();
    vector<EntityId> m_ammoEmptyEntityIds = vector<EntityId>();
    vector<EntityId> m_ammoFilledEntityIds = vector<EntityId>();

    vector<EntityId> m_bulletPool = vector<EntityId>();
    size_t m_BulletPoolIndex = 0;
    const int m_bulletPoolSize = 30;
    vector<EntityId> m_enemyPool = vector<EntityId>();
    size_t m_enemyPoolIndex = 0;
    const int m_enemyPoolSize = 30;
    vector<EntityId> m_lightningStrikePool = vector<EntityId>();
    const size_t m_lightningStrikePoolSize = 10;
    size_t m_lightningStrikePoolIndex = 0;

    void InitAmmoEntities();
    void InitBulletPool();
    void InitEnemyPool();
    void InitLightningStrikePool();
    
    EntityId CreateEntityId();
    EntityId CreatePlayerEntity(SpriteManager &spriteManager);
    EntityId CreateEnemyEntity(SpriteManager &spriteManager);
    EntityId CreateBulletEntity(SpriteManager &spriteManager, const vec3 &pos, const vec2 &targetVelocity);
    EntityId CreatePickupEntity(SpriteManager& spriteManager, const char* spritePath, PickupType pickupType, vec3 scale, float radiusMultiplier = 1.0f);
    EntityId CreateAmmoPickupEntity(SpriteManager& spriteManager);
    EntityId CreateHealthPickupEntity(SpriteManager& spriteManager);
    EntityId CreateLightningPickupEntity(SpriteManager& spriteManager);
    EntityId CreateAmmoEntity(SpriteManager &spriteManager, AmmoType ammoType, float xPos, float yPos);
    EntityId CreateHealthBarEntity(SpriteManager &spriteManager);
    EntityId CreateScoreEntity();
    EntityId CreateCountdownTimerEntity();
    EntityId CreateTitleEntity(SpriteManager &spriteManager);
    EntityId CreateButtonEntity(SpriteManager& spriteManager, const char* spritePath, GameState gameState);
    EntityId CreatePlayButtonEntity(SpriteManager &spriteManager);
    EntityId CreateBackButtonEntity(SpriteManager &spriteManager);
    EntityId CreateQuitButtonEntity(SpriteManager &spriteManager);
    EntityId CreateLoadingScreenCharacterEntity(SpriteManager &spriteManager);
    EntityId CreateStarfieldEntity(SpriteManager &spriteManager);
    EntityId CreateCrosshairEntity(SpriteManager& spriteManager);
};
