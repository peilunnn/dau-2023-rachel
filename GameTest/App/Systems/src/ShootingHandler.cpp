#include "stdafx.h"
#include "../include/ShootingHandler.h"
#include <glm/glm.hpp>

int ShootingHandler::bulletCount = 0;

void ShootingHandler::Shoot(EntityManager &entityManager, Entity playerEntityId, std::shared_ptr<CSimpleSprite> bulletSprite, float mouseX, float mouseY)
{
    if (bulletCount < MAX_BULLETS)
    {
        auto playerTransform = entityManager.GetComponent<Transform>(playerEntityId);

        if (playerTransform)
        {
            glm::vec3 bulletPos = playerTransform->position;

            glm::vec2 direction = glm::normalize(glm::vec2(mouseX, mouseY) - glm::vec2(bulletPos.x, bulletPos.y));
            if (glm::length(direction) == 0)
            {
                direction = glm::vec2(1.0f, 0.0f);
            }

            float bulletSpeed = 0.7f;
            glm::vec2 bulletVelocity = direction * bulletSpeed;

            entityManager.CreateBulletEntity(bulletSprite, bulletPos, bulletVelocity);
            bulletCount++;
        }
    }
}
