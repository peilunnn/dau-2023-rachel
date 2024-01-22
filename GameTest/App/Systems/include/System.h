#pragma once
#include "Utilities/include/Event.h"
#include <set>
using std::set;

class System
{
public:
	virtual ~System() = default;
	virtual void HandleEvent(const Event& event, float deltaTime);
	virtual set<EventType> GetSubscribedEvents() const { return m_subscribedEvents; };

protected:
	set<EventType> m_subscribedEvents;
};