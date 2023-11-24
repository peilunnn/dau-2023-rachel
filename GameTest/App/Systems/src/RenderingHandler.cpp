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

        if (!(transform && renderable))
            continue;

        renderable->sprite->SetPosition(transform->position.x, transform->position.y);
        renderable->sprite->SetScale(transform->scale.x);
        renderable->sprite->SetAngle(transform->rotation.z);

        if (tag->entityType == EntityType::AMMO_FILLED)
        {
            Helper::Log("found ammo_filled via tag");
            bool isVisible = renderable->sprite->GetIsVisible();
            if (!isVisible)
                continue;
        }

        renderable->sprite->Draw();
    }
}
