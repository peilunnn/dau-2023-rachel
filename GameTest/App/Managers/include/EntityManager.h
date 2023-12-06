#pragma once
#include "Components/include/Component.h"
#include "Utilities/include/SimpleSprite.h"
#include "Systems/include/Event.h"
#include <glm/glm.hpp>
#include <memory>
#include <typeindex>
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

    void Init(CSimpleSprite* playerSprite, CSimpleSprite* enemySprite, CSimpleSprite* reloadingCircleSprite, CSimpleSprite* healthBarSprite, vector<CSimpleSprite*> ammoEmptySprites, vector<CSimpleSprite*> ammoFilledSprites);
    vector<EntityId> GetAllEntities();
    EntityId CreateEntityId();
    EntityId CreatePlayerEntity(CSimpleSprite* playerSprite);
    EntityId CreateEnemyEntity(const vec3 &playerPos, CSimpleSprite* enemySprite, float screenWidth, float screenHeight);
    EntityId CreateBulletEntity(CSimpleSprite* bulletSprite, const vec3 &pos, const vec2 &targetVelocity);
    EntityId CreateReloadingCircleEntity(CSimpleSprite* reloadingCircleSprite);
    EntityId CreateAmmoEntity(CSimpleSprite* sprite, EntityType entityType, float xPos, float yPos);
    EntityId CreateHealthBarEntity(CSimpleSprite* sprite, float xPos, float yPos);
    EntityId CreateScoreEntity();
    EntityId CreateTimerEntity();
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

    EntityId m_nextEntityId = -1;
    unordered_map<EntityId, vector<unique_ptr<Component>>> m_entityComponents;
    vector<EntityId> m_entitiesToDelete;
    vector<EntityId> m_ammoEmptyEntities;
    vector<EntityId> m_ammoFilledEntities;
};
