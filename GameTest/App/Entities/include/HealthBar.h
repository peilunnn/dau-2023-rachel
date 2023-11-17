#ifndef HEALTHBAR_H
#define HEALTHBAR_H

#include "EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Bar.h"

Entity CreateHealthBar(EntityManager& entityManager, int currentValue, int maxValue, const glm::vec3& foregroundColor, const glm::vec3& backgroundColor, const glm::vec3& position);

#endif // HEALTHBAR_H
