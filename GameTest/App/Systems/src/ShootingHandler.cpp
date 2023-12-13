#include "stdafx.h"
#include "Components/include/Cooldown.h"
#include "Components/include/Transform.h"
#include "Components/include/Velocity.h"
#include "Managers/include/SoundManager.h"
#include "Systems/include/RenderingHandler.h"
#include "Systems/include/ShootingHandler.h"
#include "Utilities/include/app.h"
#include "Utilities/include/Helper.h"
using glm::vec2;
using glm::vec3;

ShootingHandler& ShootingHandler::GetInstance()
{
	static ShootingHandler instance;
	return instance;
}

void ShootingHandler::HandleEvent(const Event& event, float deltaTime)
{
	EntityManager& entityManager = EntityManager::GetInstance();

	if (event.GetEventType() == EventType::PlayerShoot) {
		HandlePlayerShoot(entityManager);
	}
	else if (event.GetEventType()  == EventType::PlayerHitReloadingCircle) {
		HandlePlayerHitReloadingCircle(entityManager, deltaTime);
	}
}

void ShootingHandler::ResetBullets()
{
	RenderingHandler& renderingHandler = RenderingHandler::GetInstance();
	m_bulletsShotSoFar = 0;
	renderingHandler.ShowAllAmmoFilledEntities();
}

void ShootingHandler::HandlePlayerShoot(EntityManager& entityManager)
{
	SpriteManager& spriteManager = SpriteManager::GetInstance();
	SoundManager& soundManager = SoundManager::GetInstance();
	RenderingHandler& renderingHandler = RenderingHandler::GetInstance();
	EntityId playerEntityId = entityManager.GetPlayerEntityId();
	float mouseX, mouseY;
	App::GetMousePos(mouseX, mouseY);

	Cooldown* cooldown = entityManager.GetComponent<Cooldown>(playerEntityId);
	Transform* playerTransform = entityManager.GetComponent<Transform>(playerEntityId);

	if (cooldown->IsCooldownComplete() && m_bulletsShotSoFar < MAX_BULLETS && playerTransform)
	{
		soundManager.PlaySoundFromFile(Helper::PATH_TO_GUNFIRE);

		EntityId bulletEntityId = entityManager.GetBulletFromPool();
		Transform* bulletTransform = entityManager.GetComponent<Transform>(bulletEntityId);
		Velocity* bulletVelocityComponent = entityManager.GetComponent<Velocity>(bulletEntityId);

		vec3 bulletPos = playerTransform->GetPosition();
		vec2 bulletDirection = normalize(vec2(mouseX, mouseY) - vec2(bulletPos.x, bulletPos.y));

		if (dot(bulletDirection, bulletDirection) == 0)
			bulletDirection = vec2(1.0f, 0.0f);
		else
			bulletDirection = normalize(bulletDirection);

		vec2 bulletVelocity = bulletDirection * m_bulletSpeed;
		bulletVelocityComponent->SetVelocity(bulletVelocity);
		bulletTransform->SetPosition(bulletPos);

		m_bulletsShotSoFar++;
		renderingHandler.HideAmmoFilledEntity(m_bulletsShotSoFar - 1);

		cooldown->StartCooldown();
	}
}

void ShootingHandler::HandlePlayerHitReloadingCircle(EntityManager& entityManager, float deltaTime)
{
	SoundManager::GetInstance().PlaySoundFromFile(Helper::PATH_TO_RELOAD);
	RenderingHandler& renderingHandler = RenderingHandler::GetInstance();

	EntityId reloadingCircleEntityId = entityManager.GetReloadingCircleEntityId();
	m_bulletsShotSoFar = 0;
	renderingHandler.ShowAllAmmoFilledEntities();
	entityManager.MoveEntityToRandomPos(reloadingCircleEntityId);
}