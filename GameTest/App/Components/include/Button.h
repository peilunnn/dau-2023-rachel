#pragma once
#include "Component.h"
#include <functional>
using namespace std;

struct Button : public Component {
public:
	Button(function<void()> click) : onClick(click) {}

private:
	bool isHovered = false;
	bool isPressed = false;
	function<void()> onClick;
};