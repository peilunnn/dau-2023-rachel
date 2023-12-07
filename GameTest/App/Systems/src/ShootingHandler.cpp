#include "stdafx.h"
#include "Components/include/Cooldown.h"
#include "Components/include/Transform.h"
#include "Systems/include/ShootingHandler.h"
using glm::vec2;
using glm::vec3;

void ShootingHandler::Shoot(EntityManager& entityManager, EntityId playerEntityId, float mouseX, float mouseY)
{
	SpriteManager& spriteManager = SpriteManager::GetInstance();
	Cooldown* cooldown = entityManager.GetComponent<Cooldown>(playerEntityId);
	Transform* playerTransform = entityManager.GetComponent<Transform>(playerEntityId);

	if (cooldown->IsCooldownComplete() && m_bulletsShotSoFar < MAX_BULLETS && playerTransform)
	{
		vec3 bulletPos = playerTransform->GetPosition();
		vec2 direction = normalize(vec2(mouseX, mouseY) - vec2(bulletPos.x, bulletPos.y));

		if (dot(direction, direction) == 0)
			direction = vec2(1.0f, 0.0f);
		else
			direction = normalize(direction);

		vec2 bulletVelocity = direction * m_bulletSpeed;
		entityManager.CreateBulletEntity(spriteManager, bulletPos, bulletVelocity);

		m_bulletsShotSoFar++;
		entityManager.HideAmmoFilledEntity(m_bulletsShotSoFar - 1);

		cooldown->StartCooldown();
	}
}

void ShootingHandler::HandleEvent(const Event& event, float deltaTime)
{
	EntityManager& entityManager = EntityManager::GetInstance();

	if (event.GetEventType()  == "PlayerHitReloadingCircle") {
		HandlePlayerHitReloadingCircle(entityManager, deltaTime);
	}
}

void ShootingHandler::HandlePlayerHitReloadingCircle(EntityManager& entityManager, float deltaTime)
{
	EntityId reloadingCircleEntityId = entityManager.GetReloadingCircleEntityId();
	m_bulletsShotSoFar = 0;
	entityManager.ShowAllAmmoFilledEntity();
	entityManager.MoveEntityToRandomPos(reloadingCircleEntityId);
}
