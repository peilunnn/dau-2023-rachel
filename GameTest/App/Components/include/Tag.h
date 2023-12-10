#pragma once
#include "Components/include/Component.h"
#include "Utilities/include/Enums.h"

class Tag : public Component
{
public:
    Tag(EntityType type = EntityType::None, GameState gameState = GameState::None)
        : m_entityType(type), m_gameState(gameState) {}

    EntityType GetEntityType() const { return m_entityType; }
    EntityState GetEntityState() const { return m_entityState; }
    GameState GetCurrentGameState() const { return m_gameState; }

    void SetEntityType(EntityType newType) { m_entityType = newType; }
    void SetEntityState(EntityState newState) { m_entityState = newState; }

private:
    EntityType m_entityType = EntityType::None;
    EntityState m_entityState = EntityState::Alive;
    GameState m_gameState = GameState::None;
};