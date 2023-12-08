#pragma once
#include "Components/include/Component.h"
#include "Components/include/EntityId.h"
#include "Managers/include/SpriteManager.h"
#include "Utilities/include/Enums.h"
#include <glm/fwd.hpp>
#include <memory>
#include <unordered_map>
#include <vector>
using namespace std;
using glm::vec2;
using glm::vec3;

class EntityManager
{
public:
    static EntityManager& GetInstance() {
        static EntityManager instance;
        return instance;
    }
    EntityManager(EntityManager const&) = delete;
    void operator=(EntityManager const&) = delete;

    void Init();
    vector<EntityId> GetAllEntities();
    EntityId CreateEntityId();
    EntityId CreatePlayerEntity(SpriteManager& spriteManager);
    EntityId CreateEnemyEntity(SpriteManager& spriteManager, const vec3 &playerPos, float screenWidth, float screenHeight);
    EntityId CreateBulletEntity(SpriteManager& spriteManager, const vec3 &pos, const vec2 &targetVelocity);
    EntityId CreateReloadingCircleEntity(SpriteManager& spriteManager);
    EntityId CreateAmmoEntity(SpriteManager& spriteManager, EntityType entityType, float xPos, float yPos);
    EntityId CreateHealthBarEntity(SpriteManager& spriteManager, float xPos, float yPos);
    EntityId CreateScoreEntity();
    EntityId CreateTimerEntity();
    EntityId CreateTitleEntity(SpriteManager& spriteManager);
    EntityId CreatePlayButtonEntity(SpriteManager& spriteManager);
    EntityId CreateBackButtonEntity(SpriteManager& spriteManager);
    EntityId CreateLoadingScreenCharacterEntity(SpriteManager& spriteManager);
    
    void HideAmmoFilledEntity(int index);
    void ShowAllAmmoFilledEntity();
    void MoveEntityToRandomPos(EntityId entityId);
    void MarkEntityForDeletion(EntityId entityId);
    void ProcessDeletions();

    EntityId GetPlayerEntityId() const { return m_playerEntityId; }
    EntityId GetEnemyEntityId() const { return m_enemyEntityId; }
    EntityId GetReloadingCircleEntityId() const { return m_reloadingCircleEntityId; }
    EntityId GetHealthBarEntityId() const { return m_healthBarEntityId; }
    EntityId GetScoreEntityId() const { return m_scoreEntityId; }
    EntityId GetTimerEntityId() const { return m_timerEntityId; }
    EntityId GetTitleEntityId() const { return m_titleEntityId; }
    EntityId GetPlayButtonEntityId() const { return m_playButtonEntityId; }
    EntityId GetBackButtonEntityId() const { return m_backButtonEntityId; }
    EntityId GetLoadingScreenCharacterEntityId() const { return m_loadingScreenCharacterEntityId; }
    
    vector<EntityId> GetAmmoEmptyEntityIds() const {
        return m_ammoEmptyEntityIds;
    }
    vector<EntityId> GetAmmoFilledEntityIds() const {
        return m_ammoFilledEntityIds;
    }

    template <typename T>
    void AddComponent(EntityId entityId, unique_ptr<T> component)
    {
        m_entityComponents[entityId].push_back(move(component));
    }

    template <typename T>
    T* GetComponent(EntityId entityId)
    {
        auto it = m_entityComponents.find(entityId);
        if (it != m_entityComponents.end()) {
            for (auto& comp : it->second) {
                if (auto casted = dynamic_cast<T*>(comp.get())) {
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

    EntityId m_playerEntityId = -1;
    EntityId m_enemyEntityId = -1;
    EntityId m_reloadingCircleEntityId = -1;
    EntityId m_ammoEmptyEntityId = -1;
    EntityId m_ammoFilledEntityId = -1;
    EntityId m_healthBarEntityId = -1;
    EntityId m_scoreEntityId = -1;
    EntityId m_timerEntityId = -1;
    EntityId m_titleEntityId = -1;
    EntityId m_playButtonEntityId = -1;
    EntityId m_backButtonEntityId = -1;
    EntityId m_loadingScreenCharacterEntityId = -1;

    EntityId m_nextEntityId = -1;
    unordered_map<EntityId, vector<unique_ptr<Component>>> m_entityComponents = {};
    vector<EntityId> m_entitiesToDelete = vector<EntityId>();
    vector<EntityId> m_ammoEmptyEntityIds = vector<EntityId>();
    vector<EntityId> m_ammoFilledEntityIds = vector<EntityId>();
};
