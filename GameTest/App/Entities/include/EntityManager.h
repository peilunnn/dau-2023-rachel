#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <unordered_map>
#include <vector>
#include <typeindex>
#include <memory>
#include "../../Components/include/Component.h"

using Entity = unsigned int;


class EntityManager {
private:
    static Entity nextEntityId;
    std::unordered_map<Entity, std::vector<std::shared_ptr<Component>>> entityComponents;

public:
    static Entity CreateEntity() {
        return nextEntityId++;
    }

    template <typename T>
    void AddComponent(Entity entity, std::shared_ptr<T> component) {
        entityComponents[entity].push_back(component);
    }

    template<typename T>
    std::shared_ptr<T> GetComponent(Entity entity) {
        auto it = entityComponents.find(entity);
        if (it != entityComponents.end()) {
            for (auto& comp : it->second) {
                std::shared_ptr<T> casted = std::dynamic_pointer_cast<T>(comp);
                if (casted) {
                    return casted;
                }
            }
        }
        return nullptr;
    }

    template<typename... Components>
    std::vector<Entity> GetEntitiesWithComponents() {
        std::vector<Entity> entitiesWithComponents;
        for (const auto& pair : entityComponents) {
            bool hasAllComponents = ((GetComponent<Components>(pair.first) != nullptr) && ...);
            if (hasAllComponents) {
                entitiesWithComponents.push_back(pair.first);
            }
        }
        return entitiesWithComponents;
    }
};

#endif // ENTITY_MANAGER_H
