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
#include "../../Components/include/Damage.h"
#include "../../Components/include/Animation.h"
#include "../../Utilities/include/Helper.h"

using Entity = unsigned int;

class EntityManager
{
private:
    static Entity nextEntityId;
    std::unordered_map<Entity, std::vector<std::shared_ptr<Component>>> entityComponents;
    std::vector<Entity> entitiesToDelete;

public:
    std::vector<Entity> GetAllEntities();
    static Entity CreateEntity()
    {
        return nextEntityId++;
    }
    Entity CreatePlayerEntity(std::shared_ptr<CSimpleSprite> sprite);
    Entity CreateEnemyEntity(EntityManager &entityManager, const glm::vec3 &playerPos, std::shared_ptr<CSimpleSprite> enemySprite, float screenWidth, float screenHeight);
    Entity CreateBulletEntity(std::shared_ptr<CSimpleSprite> bulletSprite, const glm::vec3 &position, const glm::vec2 &velocity);
    
    void MarkEntityForDeletion(Entity entity);
    void ProcessDeletions();

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
