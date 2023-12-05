#pragma once
#include "Systems/include/System.h"
#include "Systems/include/AnimationHandler.h"
#include "Systems/include/InputHandler.h"
#include "Systems/include/MovementHandler.h"
#include "Systems/include/RenderingHandler.h"
#include "Systems/include/CollisionHandler.h"
#include "Systems/include/HealthHandler.h"
#include "Systems/include/ScreenHandler.h"
#include "Systems/include/ScoreHandler.h"
#include "Systems/include/ShootingHandler.h"
#include "Systems/include/EntityHandler.h"
class SystemFactory
{
public:
    static vector<unique_ptr<System>> CreateAllSystems() {
        vector<unique_ptr<System>> systems;
        systems.push_back(make_unique<HealthHandler>());
        systems.push_back(make_unique<AnimationHandler>());
        systems.push_back(make_unique<CollisionHandler>());
        systems.push_back(make_unique<InputHandler>());
        systems.push_back(make_unique<MovementHandler>());
        systems.push_back(make_unique<ShootingHandler>());
        systems.push_back(make_unique<RenderingHandler>());
        systems.push_back(make_unique<ScreenHandler>());
        systems.push_back(make_unique<ScoreHandler>());
        systems.push_back(make_unique<EntityHandler>());
        return systems;
    }
};

