#pragma once

class System {
public:
    virtual ~System() = default;

    enum class Type {
        AnimationHandler,
        CollisionHandler,
        HealthHandler,
        InputHandler,
        MovementHandler,
        ShootingHandler,
        RenderingHandler,
        ScreenHandler,
        ScoreHandler,
        TimerHandler
    };
    virtual Type GetSystemType() const = 0;
};