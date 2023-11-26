#include "stdafx.h"
#include "../include/SpriteManager.h"

shared_ptr<CSimpleSprite> SpriteManager::CreateSprite(const string& path, int columns, int rows)
{
    CSimpleSprite* rawSprite = App::CreateSprite(path.c_str(), columns, rows);
    return shared_ptr<CSimpleSprite>(rawSprite);
}