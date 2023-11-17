#include "stdafx.h"
#include "../include/BulletBar.h"

Entity CreateBulletBar(EntityManager& entityManager, int currentBullets, int maxBullets, const glm::vec3& foregroundColor, const glm::vec3& backgroundColor, const glm::vec3& position)
{
    Entity bulletBar = entityManager.CreateEntity();
    entityManager.AddComponent(bulletBar, std::make_shared<Transform>(position, glm::vec3(0), glm::vec3(1)));
    entityManager.AddComponent(bulletBar, std::make_shared<Bar>(currentBullets, maxBullets, foregroundColor, backgroundColor));
    return bulletBar;
}
