#pragma once
#include "States/include/IGameState.h"

class PausedState : public IGameState
{
public:
    PausedState() = default;
    ~PausedState() override = default;

    void Enter() override;
    void Exit() override;
    void Update(float deltaTime) override;
    virtual GameState GetStateEnum() const override;
};