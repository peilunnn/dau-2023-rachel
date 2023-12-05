#pragma once
#include "../App/Systems/include/Event.h"
#include "../App/Managers/include/EntityManager.h"
#include <set>

class System
{
public:
	virtual ~System() = default;
	virtual void HandleEvent(const Event& event, EntityManager& entityManager, float deltaTime) {
		// Default implementation does nothing so that only systems that actually need to handle events will override
	}
	virtual set<string> GetSubscribedEvents() const
	{
		// Default implementation returns empty set so that only systems that actually need to handle events will override
		return set<string>();
	};
};