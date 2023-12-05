#pragma once
#include "../App/Components/include/EntityId.h"
#include "../App/Utilities/include/Enums.h"
#include <string>
#include <vector>
using namespace std;

class Event {
public:
	Event(const string& type, initializer_list<EntityId> ents)
		: m_eventType(type), m_entityIds(ents) {}
	
	string GetEventType() const {
		return m_eventType;
	}

	vector<EntityId> GetEntities() const {
		return m_entityIds;
	}
	void SetEventType(const string& newType) {
		m_eventType = newType;
	}
	void SetEntities(const vector<EntityId>& newEntities) {
		m_entityIds = newEntities;
	}

private:
	string m_eventType;
	vector<EntityId> m_entityIds;
};