#include "stdafx.h"
#include "../include/RenderingHandler.h"
#include "../../Utilities/include/Helper.h"

void RenderingHandler::Render(EntityManager &entityManager)
{
    for (auto entity : entityManager.GetEntitiesWithComponents<Tag, Transform, Renderable>())
    {
        auto tag = entityManager.GetComponent<Tag>(entity);
        auto transform = entityManager.GetComponent<Transform>(entity);
        auto renderable = entityManager.GetComponent<Renderable>(entity);
        auto animation = entityManager.GetComponent<Animation>(entity);

        if (transform && renderable)
        {
            renderable->sprite->SetPosition(transform->position.x, transform->position.y);
            renderable->sprite->SetScale(transform->scale.x);
            renderable->sprite->SetAngle(transform->rotation.z);
            renderable->sprite->Draw();
        }
    }
}
