#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include <memory>
#include "../App/Utilities/include/SimpleSprite.h"
#include "../App/Utilities/include/App.h"
#include "../App/Utilities/include/Helper.h"

class SpriteManager {
public:
    static std::shared_ptr<CSimpleSprite> CreateSprite(const std::string& path, int columns, int rows);

};

#endif // SPRITE_MANAGER_H