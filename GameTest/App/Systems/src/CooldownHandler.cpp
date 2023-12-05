#include "stdafx.h"
#include "Components/include/Cooldown.h"
#include "Components/include/Tag.h"
#include "Managers/include/EntityManager.h"
#include "Utilities/include/Helper.h"
#include "../include/CooldownHandler.h"

void CooldownHandler::Update(float deltaTime) {
    EntityManager& entityManager = EntityManager::GetInstance();

    for (EntityId entityId : entityManager.GetEntitiesWithComponents<Cooldown>()) {
        shared_ptr<Cooldown> cooldown = entityManager.GetComponent<Cooldown>(entityId);
        
        if (!cooldown->IsActive())
            return;
        
        cooldown->Update(deltaTime);
    }
}