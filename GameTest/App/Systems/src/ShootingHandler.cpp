#include "stdafx.h"
#include "Components/include/Transform.h"
#include "Components/include/Velocity.h"
#include "Utilities/include/app.h"
#include "../App/Utilities/include/Enums.h"
#include "../include/ShootingHandler.h"
#include <glm/glm.hpp>

int ShootingHandler::s_bulletsShotSoFar = 0;
float ShootingHandler::s_timeSinceLastShot = 0.0f;

void ShootingHandler::Shoot(EntityManager& entityManager, EntityId playerEntityId, shared_ptr<CSimpleSprite> bulletSprite, float mouseX, float mouseY)
{
	auto playerTransform = entityManager.GetComponent<Transform>(playerEntityId);

	if (!(s_bulletsShotSoFar < MAX_BULLETS && s_timeSinceLastShot >= s_cooldownTimer && playerTransform))
		return;

	glm::vec3 bulletPos = playerTransform->GetPosition();
	glm::vec2 direction = glm::normalize(glm::vec2(mouseX, mouseY) - glm::vec2(bulletPos.x, bulletPos.y));
	if (glm::length(direction) == 0)
		direction = glm::vec2(1.0f, 0.0f);
	float bulletSpeed = 1500.0f;
	glm::vec2 bulletVelocity = direction * bulletSpeed;
	entityManager.CreateBulletEntity(bulletSprite, bulletPos, bulletVelocity);

	s_bulletsShotSoFar++;
	s_timeSinceLastShot = 0.0f;
	entityManager.HideAmmoFilledEntity(s_bulletsShotSoFar - 1);
}

void ShootingHandler::ProcessPlayerHitReloadingCircle(EntityManager& entityManager, float deltaTime)
{
	auto reloadingCircleEntity = entityManager.GetReloadingCircleEntityId();
	s_bulletsShotSoFar = 0;
	entityManager.ShowAllAmmoFilledEntity();
	entityManager.MoveEntityToRandomPos(reloadingCircleEntity);
}
