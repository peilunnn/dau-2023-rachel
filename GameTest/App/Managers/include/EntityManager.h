#pragma once
#include <unordered_map>
#include <vector>
#include <typeindex>
#include <memory>
#include "../../Components/include/Tag.h"
#include "../../Components/include/Component.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Collider.h"
#include "../../Components/include/Renderable.h"
#include "../../Components/include/Velocity.h"
#include "../../Components/include/Direction.h"
#include "../../Components/include/Health.h"
#include "../../Components/include/Animation.h"
#include "../../Components/include/Score.h"
#include "../../Components/include/Timer.h"
#include "../../Components/include/EntityID.h"
#include "../../Systems/include/Event.h"
#include "../../Systems/include/ScreenHandler.h"
#include "../../Utilities/include/Helper.h"
#include "../../Utilities/include/App.h"
#include "../../Managers/include/SpriteManager.h"

class EntityManager
{
private:
    EntityId playerEntity;
    EntityId enemyEntity;
    EntityId reloadingCircleEntity;
    EntityId ammoEmptyEntity;
    EntityId ammoFilledEntity;
    EntityId healthBarEntity;
    EntityId scoreEntity;
    EntityId timerEntity;

    static EntityId nextEntityId;
    std::unordered_map<EntityId, std::vector<std::shared_ptr<Component>>> entityComponents;
    std::vector<EntityId> entitiesToDelete;
    std::vector<EntityId> ammoEmptyEntities;
    std::vector<EntityId> ammoFilledEntities;

public:
    EntityId GetPlayerEntityId() const
    {
        return playerEntity;
    }
    EntityId GetEnemyEntityId() const
    {
        return enemyEntity;
    }
    EntityId GetReloadingCircleEntityId() const
    {
        return reloadingCircleEntity;
    }
    EntityId GetHealthBarEntityId() const
    {
        return healthBarEntity;
    }
    EntityId GetScoreEntityId() const
    {
        return scoreEntity;
    }
    EntityId GetTimerEntityId() const
    {
        return timerEntity;
    }

    void Init(std::shared_ptr<CSimpleSprite> playerSprite, std::shared_ptr<CSimpleSprite> enemySprite, std::shared_ptr<CSimpleSprite> reloadingCircleSprite, std::shared_ptr<CSimpleSprite> ammoEmptySprite, std::shared_ptr<CSimpleSprite> ammoFilledSprite, std::shared_ptr<CSimpleSprite> healthBarSprite);
    std::vector<EntityId> GetAllEntities();
    static EntityId CreateEntity();
    EntityId CreatePlayerEntity(std::shared_ptr<CSimpleSprite> playerSprite);
    EntityId CreateEnemyEntity(const glm::vec3 &playerPos, std::shared_ptr<CSimpleSprite> enemySprite, float screenWidth, float screenHeight);
    EntityId CreateBulletEntity(std::shared_ptr<CSimpleSprite> bulletSprite, const glm::vec3 &position, const glm::vec2 &targetVelocity);
    EntityId CreateReloadingCircleEntity(std::shared_ptr<CSimpleSprite> reloadingCircleSprite);
    EntityId CreateAmmoEntity(std::shared_ptr<CSimpleSprite> sprite, EntityType entityType, float xPos, float yPos);
    EntityId CreateHealthBarEntity(std::shared_ptr<CSimpleSprite> sprite, float xPos, float yPos);
    
    EntityId CreateScoreEntity();
    EntityId CreateTimerEntity();

    void HideAmmoFilledEntity(int index);
    void ShowAllAmmoFilledEntity();
    void MoveEntityToRandomPos(EntityId entityId);
    void MarkEntityForDeletion(EntityId entityId);
    void ProcessDeletions();

    void ProcessBulletHitEnemy(EntityManager& entityManager, Event event, float deltaTime, const glm::vec3& playerPos, float screenWidth, float screenHeight);
    void ProcessEnemyHitPlayer(EntityManager& entityManager, Event event, float deltaTime);

    template <typename T>
    void AddComponent(EntityId entityId, std::shared_ptr<T> component)
    {
        entityComponents[entityId].push_back(component);
    }

    template <typename T>
    std::shared_ptr<T> GetComponent(EntityId entityId)
    {
        auto it = entityComponents.find(entityId);
        if (it != entityComponents.end())
        {
            for (auto &comp : it->second)
            {
                std::shared_ptr<T> casted = std::dynamic_pointer_cast<T>(comp);
                if (casted)
                {
                    return casted;
                }
            }
        }
        return nullptr;
    }

    template <typename... Components>
    std::vector<EntityId> GetEntitiesWithComponents()
    {
        std::vector<EntityId> entitiesWithComponents;
        for (const auto &pair : entityComponents)
        {
            bool hasAllComponents = ((GetComponent<Components>(pair.first) != nullptr) && ...);
            if (hasAllComponents)
            {
                entitiesWithComponents.push_back(pair.first);
            }
        }
        return entitiesWithComponents;
    }
};