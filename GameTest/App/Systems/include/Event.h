#pragma once
#include "../App/Components/include/EntityId.h"
#include "../App/Utilities/include/Enums.h"
#include <string>
#include <vector>
using namespace std;

class Event {
public:
	string eventType;
	vector<EntityId> entities;

	Event(const string& type, initializer_list<EntityId> ents)
		: eventType(type), entities(ents) {}
};