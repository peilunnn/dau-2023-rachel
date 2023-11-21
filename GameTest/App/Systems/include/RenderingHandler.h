#ifndef RENDER_ENTITIES_H
#define RENDER_ENTITIES_H

#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Renderable.h"
#include "../../Components/include/Animation.h"
#include "../../Systems/include/System.h"

class RenderingHandler : public System {
public:
    void Render(EntityManager& entityManager);
    Type GetSystemType() const override {
        return System::Type::RenderingHandler;
    }
};

#endif // RENDER_ENTITIES_H
