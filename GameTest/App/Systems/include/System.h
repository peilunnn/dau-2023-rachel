#pragma once
#include "Systems/include/Event.h"
#include <set>

class System
{
public:
	virtual ~System() = default;
	virtual void HandleEvent(const Event& event, float deltaTime) {
		// Default implementation does nothing so that only systems that actually need to handle events will override
	}
	virtual set<EventType> GetSubscribedEvents() const
	{
		// Default implementation returns empty set so that only systems that actually need to handle events will override
		return set<EventType>();
	};
};