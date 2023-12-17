#pragma once
#include "Components/include/EntityId.h"
#include "Utilities/include/Enums.h"
#include <vector>
using std::vector;
using std::initializer_list;

class Event {
public:
	Event(const EventType type, initializer_list<EntityId> ents)
		: m_eventType(type), m_entityIds(ents) {}
	
	EventType GetEventType() const {
		return m_eventType;
	}
	vector<EntityId> GetEntities() const {
		return m_entityIds;
	}

private:
	EventType m_eventType = EventType::None;
	vector<EntityId> m_entityIds = vector<EntityId>();
};