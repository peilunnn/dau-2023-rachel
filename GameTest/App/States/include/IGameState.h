#pragma once
#include "Utilities/include/EntityId.h"

class GameManager;

class IGameState 
{
public:
    virtual ~IGameState() = default;

    virtual void Enter() = 0;
    virtual void Exit() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void HandleButtonClick() = 0;
    virtual GameState GetStateEnum() const = 0;
};