#pragma once
#include "States/include/IGameState.h"

class MainMenuState : public IGameState
{
public:
    MainMenuState() = default;
    ~MainMenuState() override = default;

    void Enter() override;
    void Exit() override;
    void Update(float deltaTime) override;
    virtual GameState GetStateEnum() const override;
};