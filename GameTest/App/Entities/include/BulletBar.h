#ifndef BULLETBAR_H
#define BULLETBAR_H

#include "EntityManager.h"
#include "../../Components/include/Transform.h"
#include "../../Components/include/Bar.h"

Entity CreateBulletBar(EntityManager& entityManager, int currentBullets, int maxBullets, const glm::vec3& foregroundColor, const glm::vec3& backgroundColor, const glm::vec3& position) {
    Entity bulletBar = entityManager.CreateEntity();
    entityManager.AddComponent(bulletBar, Bar(currentBullets, maxBullets, foregroundColor, backgroundColor));
    entityManager.AddComponent(bulletBar, Transform(position, glm::vec3(0), glm::vec3(1)));
    return bulletBar;
}

#endif // BULLETBAR_H
