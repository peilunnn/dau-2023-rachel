#ifndef RENDERABLE_H
#define RENDERABLE_H

// TODO: move everything in Sprite, Sound, Controller, Application into either E or C or S
#include "../../Sprite/include/SimpleSprite.h"

struct Renderable {
    CSimpleSprite* sprite;

    Renderable(CSimpleSprite* sprite) : sprite(sprite) {}
};

#endif // RENDERABLE_H
