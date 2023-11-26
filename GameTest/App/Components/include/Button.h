#pragma once
#include "Component.h"
#include <functional>
using namespace std;

struct Button : public Component {
	bool isHovered = false;
	bool isPressed = false;
	function<void()> onClick;

	Button(function<void()> click) : onClick(click) {}
};