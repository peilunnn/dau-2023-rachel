#pragma once
#include "Components/include/Component.h"
#include "Utilities/include/Enums.h"

class Tag : public Component
{
public:
    Tag(EntityType newType) : m_entityType(newType) {}

    EntityType GetEntityType() const { return m_entityType; }
    EntityState GetEntityState() const { return m_entityState; }

    void SetEntityType(EntityType newType) { m_entityType = newType; }
    void SetEntityState(EntityState newState) { m_entityState = newState; }

private:
    EntityType m_entityType = EntityType::None;
    EntityState m_entityState = EntityState::Alive;
};