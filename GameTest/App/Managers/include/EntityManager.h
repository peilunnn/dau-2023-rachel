#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

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
#include "../../Systems/include/Event.h"
#include "../../Systems/include/ScreenHandler.h"
#include "../../Utilities/include/Helper.h"
#include "../../Utilities/include/App.h"
#include "../../Managers/include/SpriteManager.h"

using Entity = unsigned int;

class EntityManager
{
private:
    Entity playerEntity;
    Entity enemyEntity;
    Entity reloadingCircleEntity;
    Entity ammoEmptyEntity;
    Entity ammoFilledEntity;
    Entity healthBarEntity;
    Entity scoreEntity;

    static Entity nextEntityId;
    std::unordered_map<Entity, std::vector<std::shared_ptr<Component>>> entityComponents;
    std::vector<Entity> entitiesToDelete;
    std::vector<Entity> ammoEmptyEntities;
    std::vector<Entity> ammoFilledEntities;

public:
    Entity GetPlayerEntity() const
    {
        return playerEntity;
    }
    Entity GetEnemyEntity() const
    {
        return enemyEntity;
    }
    Entity GetReloadingCircleEntity() const
    {
        return reloadingCircleEntity;
    }
    Entity GetHealthBarEntity() const
    {
        return healthBarEntity;
    }
    Entity GetScoreEntity() const
    {
        return scoreEntity;
    }

    void Init(std::shared_ptr<CSimpleSprite> playerSprite, std::shared_ptr<CSimpleSprite> enemySprite, std::shared_ptr<CSimpleSprite> reloadingCircleSprite, std::shared_ptr<CSimpleSprite> ammoEmptySprite, std::shared_ptr<CSimpleSprite> ammoFilledSprite, std::shared_ptr<CSimpleSprite> healthBarSprite);
    std::vector<Entity> GetAllEntities();
    static Entity CreateEntity();
    Entity CreatePlayerEntity(std::shared_ptr<CSimpleSprite> playerSprite);
    Entity CreateEnemyEntity(const glm::vec3 &playerPos, std::shared_ptr<CSimpleSprite> enemySprite, float screenWidth, float screenHeight);
    Entity CreateBulletEntity(std::shared_ptr<CSimpleSprite> bulletSprite, const glm::vec3 &position, const glm::vec2 &targetVelocity);
    Entity CreateReloadingCircleEntity(std::shared_ptr<CSimpleSprite> reloadingCircleSprite);
    Entity CreateAmmoEntity(std::shared_ptr<CSimpleSprite> sprite, EntityType entityType, float xPos, float yPos);
    Entity CreateHealthBarEntity(std::shared_ptr<CSimpleSprite> sprite, float xPos, float yPos);
    
    Entity CreateScoreEntity();

    void HideAmmoFilledEntity(int index);
    void ShowAllAmmoFilledEntity();
    void MoveEntityToRandomPos(Entity entity);
    void MarkEntityForDeletion(Entity entity);
    void ProcessDeletions();

    void ProcessBulletHitEnemy(EntityManager& entityManager, Event event, float deltaTime, const glm::vec3& playerPos, float screenWidth, float screenHeight);
    void ProcessEnemyHitPlayer(EntityManager& entityManager, Event event, float deltaTime);

    template <typename T>
    void AddComponent(Entity entity, std::shared_ptr<T> component)
    {
        entityComponents[entity].push_back(component);
    }

    template <typename T>
    std::shared_ptr<T> GetComponent(Entity entity)
    {
        auto it = entityComponents.find(entity);
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
    std::vector<Entity> GetEntitiesWithComponents()
    {
        std::vector<Entity> entitiesWithComponents;
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

#endif // ENTITY_MANAGER_H
