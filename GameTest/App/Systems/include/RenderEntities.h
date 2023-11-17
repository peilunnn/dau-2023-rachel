// RenderEntitiesSystem.h
#ifndef RENDER_ENTITIES_SYSTEM_H
#define RENDER_ENTITIES_SYSTEM_H

#include "../../Entities/include/EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Renderable.h"

class RenderEntitiesSystem {
public:
    void Render(EntityManager& entityManager) {
        for (auto entity : entityManager.GetEntitiesWithComponents<Transform, Renderable>()) {
            auto* transform = entityManager.GetComponent<Transform>(entity);
            auto* renderable = entityManager.GetComponent<Renderable>(entity);

            if (transform && renderable) {
                renderable->sprite->SetPosition(transform->position.x, transform->position.y);
                renderable->sprite->SetScale(transform->scale.x);
                renderable->sprite->SetAngle(transform->rotation.z);
                renderable->sprite->Draw();
            }
        }
    }
};

#endif // RENDER_ENTITIES_SYSTEM_H
