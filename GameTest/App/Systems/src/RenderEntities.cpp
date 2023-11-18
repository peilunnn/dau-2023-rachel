#include "stdafx.h"
#include "../include/RenderEntities.h"
#include "../../Utilities/include/Debug.h"

void RenderEntities::Render(EntityManager& entityManager) {
    for (auto entity : entityManager.GetEntitiesWithComponents<Transform, Renderable>()) {
        auto transform = entityManager.GetComponent<Transform>(entity);
        auto renderable = entityManager.GetComponent<Renderable>(entity);
        auto animation = entityManager.GetComponent<Animation>(entity);

        if (transform && renderable) {
            renderable->sprite->SetPosition(transform->position.x, transform->position.y);
    
            renderable->sprite->SetScale(transform->scale.x);
            renderable->sprite->SetAngle(transform->rotation.z);
            
            if (animation) {
                renderable->sprite->SetAnimation(animation->currentAnimation);
            }

            renderable->sprite->Draw();
        }
    }
}
