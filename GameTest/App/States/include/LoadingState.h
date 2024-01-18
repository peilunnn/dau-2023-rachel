#pragma once
#include "States/include/IGameState.h"

class LoadingState : public IGameState
{
public:
    explicit LoadingState(GameState previousState);
    ~LoadingState() override = default;

    void Enter() override;
    void Exit() override;
    void Update(float deltaTime) override;
    virtual GameState GetStateEnum() const override;

private:
    float m_timeSpentInLoading = 0.0f;
    const float m_loadingDuration = 1.5f;
};
