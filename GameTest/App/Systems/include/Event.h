#pragma once
#include "../App/Components/include/EntityId.h"
#include "../App/Utilities/include/Enums.h"
#include <vector>
using namespace std;

class Event {
public:
	EventType eventType;
	vector<EntityId> entities;

	Event(EventType type, initializer_list<EntityId> ents)
		: eventType(type), entities(ents) {}
};