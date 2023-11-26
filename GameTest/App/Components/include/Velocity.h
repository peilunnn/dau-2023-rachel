#pragma once
#include "Component.h"
#include <glm/vec2.hpp>

class Velocity : public Component {
public:
	glm::vec2 velocity = glm::vec2(0.0f);

	Velocity(const glm::vec2& vel = glm::vec2(0.0f))
		: velocity(vel) {}
};