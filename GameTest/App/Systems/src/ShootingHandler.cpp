#include "stdafx.h"
#include "Components/include/Transform.h"
#include "Utilities/include/app.h"
#include "../App/Utilities/include/Enums.h"
#include "../include/ShootingHandler.h"
using glm::vec2;
using glm::vec3;

int ShootingHandler::s_bulletsShotSoFar = 0;
float ShootingHandler::s_timeSinceLastShot = 0.0f;

void ShootingHandler::Shoot(EntityManager& entityManager, EntityId playerEntityId, shared_ptr<CSimpleSprite> bulletSprite, float mouseX, float mouseY)
{
	shared_ptr<Transform> playerTransform = entityManager.GetComponent<Transform>(playerEntityId);

	if (!(s_bulletsShotSoFar < MAX_BULLETS && s_timeSinceLastShot >= s_cooldownTimer && playerTransform))
		return;

	vec3 bulletPos = playerTransform->GetPosition();
	vec2 direction = normalize(vec2(mouseX, mouseY) - vec2(bulletPos.x, bulletPos.y));

	if (dot(direction, direction) == 0)
		direction = vec2(1.0f, 0.0f);
	else
		direction = normalize(direction);

	vec2 bulletVelocity = direction * s_bulletSpeed;
	entityManager.CreateBulletEntity(bulletSprite, bulletPos, bulletVelocity);

	s_bulletsShotSoFar++;
	s_timeSinceLastShot = 0.0f;
	entityManager.HideAmmoFilledEntity(s_bulletsShotSoFar - 1);
}

void ShootingHandler::HandleEvent(const Event& event, EntityManager& entityManager, float deltaTime)
{
	if (event.GetEventType()  == "PlayerHitReloadingCircle") {
		HandlePlayerHitReloadingCircle(entityManager, deltaTime);
	}
}

void ShootingHandler::HandlePlayerHitReloadingCircle(EntityManager& entityManager, float deltaTime)
{
	EntityId reloadingCircleEntityId = entityManager.GetReloadingCircleEntityId();
	s_bulletsShotSoFar = 0;
	entityManager.ShowAllAmmoFilledEntity();
	entityManager.MoveEntityToRandomPos(reloadingCircleEntityId);
}
