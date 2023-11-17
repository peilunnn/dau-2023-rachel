#ifndef RENDER_ENTITIES_H
#define RENDER_ENTITIES_H

#include "../../Entities/include/EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Renderable.h"

class RenderEntities {
public:
    void Render(EntityManager& entityManager);
};

#endif // RENDER_ENTITIES_H
