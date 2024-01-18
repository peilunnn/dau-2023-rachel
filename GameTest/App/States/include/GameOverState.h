#pragma once
#include "States/include/IGameState.h"

class GameOverState : public IGameState
{
public:
    GameOverState() = default;
    ~GameOverState() override = default;

    void Enter() override;
    void Exit() override;
    void Update(float deltaTime) override;
    virtual GameState GetStateEnum() const override;
};