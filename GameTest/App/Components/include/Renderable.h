#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "../../Utilities/include/SimpleSprite.h"

struct Renderable {
    CSimpleSprite* sprite;

    Renderable(CSimpleSprite* sprite) : sprite(sprite) {}
};

#endif // RENDERABLE_H
