#ifndef SYSTEM_H
#define SYSTEM_H

#include "../../Managers/include/EntityManager.h"

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
    };
    virtual Type GetSystemType() const = 0;
};

#endif // SYSTEM_H
