#pragma once
#include "Utilities/include/SimpleSprite.h"
#include "Components/include/EntityId.h"
#include <memory>
#include <unordered_map>
using namespace std;

class SpriteManager {
public:
    static SpriteManager& GetInstance() {
        static SpriteManager instance;
        return instance;
    }
    SpriteManager(const SpriteManager&) = delete;
    void operator=(const SpriteManager&) = delete;

    CSimpleSprite* CreateSprite(EntityId entityId, const char* path, int columns, int rows);
    CSimpleSprite* GetSprite(EntityId entityId);
    void DestroyAllSprites();

private:
    std::unordered_map<EntityId, unique_ptr<CSimpleSprite>> m_entitySprites;
    SpriteManager() = default;
};
