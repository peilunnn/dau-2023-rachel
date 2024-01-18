#pragma once
#include "States/include/IGameState.h"

class GameplayState : public IGameState
{
public:
    GameplayState() = default;
    ~GameplayState() override = default;

    void Enter() override;
    void Exit() override;
    void Update(float deltaTime) override;
    void HandleButtonClick() override;
    virtual GameState GetStateEnum() const override;
};