#pragma once
#include "Components/include/Component.h"
#include "Utilities/include/Enums.h"
#include <set>
using std::set;

class Tag : public Component
{
public:
    Tag(EntityType type = EntityType::None, set<GameState> gameStates = {})
        : m_entityType(type), m_gameStates(gameStates) {}

    EntityType GetEntityType() const { return m_entityType; }
    EntityState GetEntityState() const { return m_entityState; }
    void SetEntityType(EntityType newType) { m_entityType = newType; }
    void SetEntityState(EntityState newState) { m_entityState = newState; }
    void AddGameState(GameState state) { m_gameStates.insert(state); }
    bool ContainsGameState(GameState state) const { return m_gameStates.find(state) != m_gameStates.end(); }

private:
    EntityType m_entityType = EntityType::None;
    EntityState m_entityState = EntityState::Alive;
    set<GameState> m_gameStates = set<GameState>();
};