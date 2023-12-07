#include "stdafx.h"
#include "Components/include/Cooldown.h"
#include "Managers/include/EntityManager.h"
#include "Systems/include/CooldownHandler.h"

void CooldownHandler::Update(float deltaTime)
{
    EntityManager &entityManager = EntityManager::GetInstance();

    for (EntityId entityId : entityManager.GetEntitiesWithComponents<Cooldown>())
    {
        Cooldown *cooldown = entityManager.GetComponent<Cooldown>(entityId);

        if (!cooldown->IsActive())
            return;

        cooldown->Update(deltaTime);
    }
}