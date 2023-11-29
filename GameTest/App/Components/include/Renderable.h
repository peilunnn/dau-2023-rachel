#pragma once
#include "Utilities/include/SimpleSprite.h"
#include "Component.h"
#include <memory>
using namespace std;

struct Renderable : public Component {
	shared_ptr<CSimpleSprite> sprite = nullptr;

	Renderable(shared_ptr<CSimpleSprite> sprite) : sprite(sprite) {}
};