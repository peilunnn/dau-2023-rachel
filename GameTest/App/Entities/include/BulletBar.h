#ifndef BULLETBAR_H
#define BULLETBAR_H

#include "EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Bar.h"

Entity CreateBulletBar(EntityManager& entityManager, int currentBullets, int maxBullets, const glm::vec3& foregroundColor, const glm::vec3& backgroundColor, const glm::vec3& position);

#endif // BULLETBAR_H
