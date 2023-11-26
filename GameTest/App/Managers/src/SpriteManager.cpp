#include "stdafx.h"
#include "../include/SpriteManager.h"

std::shared_ptr<CSimpleSprite> SpriteManager::CreateSprite(const std::string& path, int columns, int rows)
{
    CSimpleSprite* rawSprite = App::CreateSprite(path.c_str(), columns, rows);
    return std::shared_ptr<CSimpleSprite>(rawSprite);
}