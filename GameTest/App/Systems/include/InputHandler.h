#pragma once
#include "../../Managers/include/EntityManager.h"
#include "../../Systems/include/System.h"
using namespace std;

class InputHandler : public System
{
public:
	static constexpr float THUMB_STICK_THRESHOLD = 0.5f;
	static constexpr float VELOCITY_MULTIPLIER = 10000.0f;

	void Update(EntityManager& entityManager, float deltaTime, EntityId playerEntity, shared_ptr<CSimpleSprite> bulletSprite);
	void HandlePositionInput(EntityManager& entityManager, float deltaTime, EntityId playerEntity);
	void HandleShootingInput(EntityManager& entityManager, EntityId playerEntity, shared_ptr<CSimpleSprite> bulletSprite);
	SystemType GetSystemType() const override
	{
		return SystemType::InputHandler;
	}
};