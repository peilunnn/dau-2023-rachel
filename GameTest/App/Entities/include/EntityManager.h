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
};

#endif // ENTITY_MANAGER_H
