#include "stdafx.h"
#include "../include/Helper.h"

SpriteDimensions Helper::GetSpriteDimensions(std::shared_ptr<CSimpleSprite> sprite, float multiplier)
{
    SpriteDimensions dimensions;
    if (sprite) {
        dimensions.width = sprite->GetWidth();
        dimensions.height = sprite->GetHeight();
        dimensions.adjustedWidth = dimensions.width * multiplier;
        dimensions.adjustedHeight = dimensions.height * multiplier;
    }
    else {
        dimensions.width = 0;
        dimensions.height = 0;
        dimensions.adjustedWidth = 0;
        dimensions.adjustedHeight = 0;
    }
    return dimensions;
}
