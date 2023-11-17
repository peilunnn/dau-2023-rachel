#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "../../Utilities/include/SimpleSprite.h"
#include "Component.h"

struct Renderable : public Component {
    CSimpleSprite* sprite;

    Renderable(CSimpleSprite* sprite) : sprite(sprite) {}
};

#endif // RENDERABLE_H
