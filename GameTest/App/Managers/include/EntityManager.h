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
    void Init(shared_ptr<CSimpleSprite> playerSprite, shared_ptr<CSimpleSprite> enemySprite, shared_ptr<CSimpleSprite> reloadingCircleSprite, shared_ptr<CSimpleSprite> ammoEmptySprite, shared_ptr<CSimpleSprite> ammoFilledSprite, shared_ptr<CSimpleSprite> healthBarSprite);
    vector<EntityId> GetAllEntities();
    static EntityId CreateEntityId();
    EntityId CreatePlayerEntity(shared_ptr<CSimpleSprite> playerSprite);
    EntityId CreateEnemyEntity(const vec3 &playerPos, shared_ptr<CSimpleSprite> enemySprite, float screenWidth, float screenHeight);
    EntityId CreateBulletEntity(shared_ptr<CSimpleSprite> bulletSprite, const vec3 &pos, const vec2 &targetVelocity);
    EntityId CreateReloadingCircleEntity(shared_ptr<CSimpleSprite> reloadingCircleSprite);
    EntityId CreateAmmoEntity(shared_ptr<CSimpleSprite> sprite, EntityType entityType, float xPos, float yPos);
    EntityId CreateHealthBarEntity(shared_ptr<CSimpleSprite> sprite, float xPos, float yPos);
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
    void AddComponent(EntityId entityId, shared_ptr<T> component)
    {
        m_entityComponents[entityId].push_back(component);
    }

    template <typename T>
    shared_ptr<T> GetComponent(EntityId entityId)
    {
        auto it = m_entityComponents.find(entityId);
        if (it != m_entityComponents.end())
        {
            for (auto &comp : it->second)
            {
                shared_ptr<T> casted = dynamic_pointer_cast<T>(comp);
                if (casted)
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
            {
                entitiesWithComponents.push_back(pair.first);
            }
        }
        return entitiesWithComponents;
    }

private:
    EntityId m_playerEntityId;
    EntityId m_enemyEntityId;
    EntityId m_reloadingCircleEntityId;
    EntityId m_ammoEmptyEntityId;
    EntityId m_ammoFilledEntityId;
    EntityId m_healthBarEntityId;
    EntityId m_scoreEntityId;
    EntityId m_timerEntityId;

    static EntityId s_nextEntityId;
    unordered_map<EntityId, vector<shared_ptr<Component>>> m_entityComponents;
    vector<EntityId> m_entitiesToDelete;
    vector<EntityId> m_ammoEmptyEntities;
    vector<EntityId> m_ammoFilledEntities;
};
