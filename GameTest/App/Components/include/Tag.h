#pragma once
#include "Components/include/Component.h"
#include "Utilities/include/Enums.h"

class Tag : public Component
{
public:
    Tag(EntityType type = EntityType::None, Scene scene = Scene::None)
        : m_entityType(type), m_scene(scene) {}

    EntityType GetEntityType() const { return m_entityType; }
    EntityState GetEntityState() const { return m_entityState; }
    Scene GetScene() const { return m_scene; }

    void SetEntityType(EntityType newType) { m_entityType = newType; }
    void SetEntityState(EntityState newState) { m_entityState = newState; }

private:
    EntityType m_entityType = EntityType::None;
    EntityState m_entityState = EntityState::Alive;
    Scene m_scene = Scene::None;
};