#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <unordered_map>
#include <vector>
#include <typeindex>
#include "../../Components/include/Component.h"

using Entity = unsigned int;


class EntityManager {
private:
    static Entity nextEntityId;
    std::unordered_map<Entity, std::vector<Component*>> entityComponents;

public:
    static Entity CreateEntity() {
        return nextEntityId++;
    }

    template <typename T>
    void AddComponent(Entity entity, T* component) {
        entityComponents[entity].push_back(component);
    }

    template<typename T>
    T* GetComponent(Entity entity) {
        auto it = entityComponents.find(entity);
        if (it != entityComponents.end()) {
            for (Component* comp : it->second) {
                if (T* casted = dynamic_cast<T*>(comp)) {
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
            bool hasAllComponents = (GetComponent<Components>(pair.first) && ...);
            if (hasAllComponents) {
                entitiesWithComponents.push_back(pair.first);
            }
        }
        return entitiesWithComponents;
    }
};

#endif // ENTITY_MANAGER_H
