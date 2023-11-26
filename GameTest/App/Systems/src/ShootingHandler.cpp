#include "stdafx.h"
#include "../include/ShootingHandler.h"
#include <glm/glm.hpp>

int ShootingHandler::bulletsShotsSoFar = 0;
float ShootingHandler::cooldownTimer = 0.5f;
float ShootingHandler::timeSinceLastShot = 0.0f;

void ShootingHandler::Shoot(EntityManager &entityManager, EntityId playerEntity, std::shared_ptr<CSimpleSprite> bulletSprite, float mouseX, float mouseY)
{
    auto playerTransform = entityManager.GetComponent<Transform>(playerEntity);

    if (!(bulletsShotsSoFar < MAX_BULLETS && timeSinceLastShot >= cooldownTimer && playerTransform))
        return;

    glm::vec3 bulletPos = playerTransform->position;
    glm::vec2 direction = glm::normalize(glm::vec2(mouseX, mouseY) - glm::vec2(bulletPos.x, bulletPos.y));
    if (glm::length(direction) == 0)
        direction = glm::vec2(1.0f, 0.0f);
    float bulletSpeed = 1500.0f;
    glm::vec2 bulletVelocity = direction * bulletSpeed;
    entityManager.CreateBulletEntity(bulletSprite, bulletPos, bulletVelocity);

    bulletsShotsSoFar++;
    timeSinceLastShot = 0.0f;
    entityManager.HideAmmoFilledEntity(bulletsShotsSoFar - 1);
}

void ShootingHandler::ProcessPlayerHitReloadingCircle(EntityManager& entityManager, float deltaTime)
{
    auto reloadingCircleEntity = entityManager.GetReloadingCircleEntityId();
    bulletsShotsSoFar = 0;
    entityManager.ShowAllAmmoFilledEntity();
    entityManager.MoveEntityToRandomPos(reloadingCircleEntity);
}
