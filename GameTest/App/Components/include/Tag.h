#pragma once
#include "../App/Utilities/include/Enums.h"
#include "Component.h"

class Tag : public Component {
public:
    Tag(EntityType newType) : m_entityType(newType) {}

    EntityType GetEntityType() const { return m_entityType; }
    EntityState GetEntityState() const { return m_entityState; }

    void SetEntityType(EntityType newType) { m_entityType = newType; }
    void SetEntityState(EntityState newState) { m_entityState = newState; }

private:
    EntityType m_entityType = EntityType::NONE;
    EntityState m_entityState = EntityState::ALIVE;
};