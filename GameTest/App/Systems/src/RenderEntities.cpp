#include "stdafx.h"
#include "../include/RenderEntities.h"

void RenderEntities::Render(EntityManager& entityManager) {
    for (auto entity : entityManager.GetEntitiesWithComponents<Transform, Renderable>()) {
        auto transform = entityManager.GetComponent<Transform>(entity);
        auto renderable = entityManager.GetComponent<Renderable>(entity);
        auto animation = entityManager.GetComponent<Animation>(entity);

        if (transform && renderable && animation) {
            renderable->sprite->SetPosition(transform->position.x, transform->position.y);
            renderable->sprite->SetScale(transform->scale.x);
            renderable->sprite->SetAngle(transform->rotation.z);
            renderable->sprite->SetAnimation(animation->currentAnimation);
            renderable->sprite->Draw();
        }
    }
}
