#pragma once
#include "Systems/include/Event.h"
#include <set>
using std::set;

class System
{
public:
	virtual ~System() = default;
	virtual void HandleEvent(const Event& event, float deltaTime) {
		// Default implementation does nothing so that only systems that actually need to handle events will override
	}
	virtual set<EventType> GetSubscribedEvents() const { return m_subscribedEvents; };

protected:
	set<EventType> m_subscribedEvents;
};