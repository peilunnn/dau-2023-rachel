#pragma once
#include <memory>
#include "../../Utilities/include/SimpleSprite.h"
#include "Component.h"

struct Renderable : public Component {
    std::shared_ptr<CSimpleSprite> sprite;

    Renderable(std::shared_ptr<CSimpleSprite> sprite) : sprite(sprite) {}
};