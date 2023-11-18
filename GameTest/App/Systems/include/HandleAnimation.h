#ifndef HANDLE_ANIMATION_H
#define HANDLE_ANIMATION_H

#include "../../Managers/include/EntityManager.h"
#include "../../Components/include/Animation.h"
#include "../../Components/include/Velocity.h"
#include "../../Utilities/include/SimpleSprite.h"

class HandleAnimation {
private:
	enum
	{
		ANIM_FORWARDS,
		ANIM_BACKWARDS,
		ANIM_LEFT,
		ANIM_RIGHT,
		ANIM_IDLE_LEFT,
		ANIM_IDLE_RIGHT,
		ANIM_IDLE_FORWARDS,
		ANIM_IDLE_BACKWARDS,
	};
	int lastNonIdleAnimState = ANIM_IDLE_FORWARDS;

public:
	void Init(CSimpleSprite* sprite);
	void Update(EntityManager& entityManager, float deltaTime);
};

#endif // HANDLE_ANIMATION_H