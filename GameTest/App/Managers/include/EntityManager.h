#pragma once
#include "Components/include/Component.h"
#include "Utilities/include/EntityId.h"
#include "Managers/include/SpriteManager.h"
#include "Utilities/include/Enums.h"
#include <glm/fwd.hpp>
#include <memory>
#include <unordered_map>
#include <vector>
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
    EntityId CreateButtonEntity(SpriteManager& spriteManager, const char* spritePath, GameState gameState, float xOffset, float yOffset, vec3 scale);
    EntityId CreatePlayButtonEntity(SpriteManager &spriteManager);
    EntityId CreateBackButtonEntity(SpriteManager &spriteManager);
    EntityId CreateQuitButtonEntity(SpriteManager &spriteManager);
    EntityId CreateLoadingScreenCharacterEntity(SpriteManager &spriteManager);
    EntityId CreateStarfieldEntity(SpriteManager &spriteManager);
    EntityId CreateCrosshairEntity(SpriteManager& spriteManager);
};
