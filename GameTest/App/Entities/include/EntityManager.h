#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <unordered_map>
#include <vector>
#include <typeindex>

using Entity = unsigned int;

struct Component;

class EntityManager {
private:
    static Entity nextEntityId;
    std::unordered_map<Entity, std::vector<Component*>> entityComponents;

public:
    static Entity CreateEntity() {
        return nextEntityId++;
    }

    template <typename T>
    void AddComponent(Entity entity, T component);
};

template<typename T>
void EntityManager::AddComponent(Entity entity, T component) {
}

#endif // ENTITY_MANAGER_H
