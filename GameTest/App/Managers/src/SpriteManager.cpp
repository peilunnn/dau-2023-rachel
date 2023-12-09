#include "stdafx.h"
#include "Managers/include/SpriteManager.h"
#include "Utilities/include/app.h"

SpriteManager& SpriteManager::GetInstance()
{
    static SpriteManager instance;
    return instance;
}

CSimpleSprite* SpriteManager::CreateSprite(EntityId entityId, const char* path, int columns, int rows)
{
    CSimpleSprite* sprite = App::CreateSprite(path, columns, rows);
    m_entitySprites[entityId] = unique_ptr<CSimpleSprite>(sprite);
    return sprite;
}

CSimpleSprite* SpriteManager::GetSprite(EntityId entityId)
{
    auto it = m_entitySprites.find(entityId);
    if (it != m_entitySprites.end()) {
        return it->second.get();
    }
    return nullptr;
}

void SpriteManager::DestroyAllSprites() {
    m_entitySprites.clear();
}