#include "stdafx.h"
#include "../include/HealthBar.h"

Entity CreateHealthBar(EntityManager& entityManager, int currentValue, int maxValue, const glm::vec3& foregroundColor, const glm::vec3& backgroundColor, const glm::vec3& position)
{
    Entity healthBar = entityManager.CreateEntity();
    entityManager.AddComponent(healthBar, std::make_shared<Bar>(currentValue, maxValue, foregroundColor, backgroundColor));
    entityManager.AddComponent(healthBar, std::make_shared<Transform>(position, glm::vec3(0), glm::vec3(1)));
    return healthBar;
}
