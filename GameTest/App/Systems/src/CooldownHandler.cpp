#include "stdafx.h"
#include "Components/include/Cooldown.h"
#include "Managers/include/EntityManager.h"
#include "Managers/include/GameManager.h"
#include "Systems/include/CooldownHandler.h"

CooldownHandler& CooldownHandler::GetInstance()
{
    static CooldownHandler instance;
    return instance;
}

void CooldownHandler::Update(float deltaTime)
{
    GameManager& gameManager = GameManager::GetInstance();
    EntityManager &entityManager = EntityManager::GetInstance();
    
    for (EntityId entityId : entityManager.GetEntitiesWithComponents<Cooldown>())
    {
        Cooldown *cooldown = entityManager.GetComponent<Cooldown>(entityId);

        if (!cooldown->IsActive())
            return;

        cooldown->Update(deltaTime);
    }
}