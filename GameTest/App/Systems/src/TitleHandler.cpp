#include "stdafx.h"
#include "Components/include/Transform.h"
#include "Managers/include/EntityManager.h"
#include "Systems/include/TitleHandler.h"

void TitleHandler::OscillateTitle(float deltaTime)
{
	EntityManager& entityManager = EntityManager::GetInstance();
	EntityId titleEntityId = entityManager.GetTitleEntityId();
	Transform* titleTransform = entityManager.GetComponent<Transform>(titleEntityId);
	m_accumulatedTime += deltaTime;

	const float amplitude = 0.3f;
	const float frequency = 4.0f;
	float titleRotationAngle = amplitude * sin(m_accumulatedTime * frequency);
	titleTransform->SetRotation(vec3(0.0f, 0.0f, titleRotationAngle));
}
