#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include "EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Bar.h"

Entity CreateHealthBar(EntityManager& entityManager, int currentValue, int maxValue, const glm::vec3& foregroundColor, const glm::vec3& backgroundColor, const glm::vec3& position) {
    Entity healthBar = entityManager.CreateEntity();
    entityManager.AddComponent(healthBar, Bar(currentValue, maxValue, foregroundColor, backgroundColor));
    entityManager.AddComponent(healthBar, Transform(position, glm::vec3(0), glm::vec3(1)));
    return healthBar;
}

#endif // HEALTHBAR_H
