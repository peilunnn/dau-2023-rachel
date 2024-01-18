#pragma once
#include "Utilities/include/EntityId.h"

class GameManager;

class IGameState 
{
public:
    virtual ~IGameState() = default;
    
    void HandleButtonClick(EntityId buttonEntityId);

    virtual void Enter() = 0;
    virtual void Exit() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual GameState GetStateEnum() const = 0;
};