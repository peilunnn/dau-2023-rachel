#pragma once
#include "Components/include/Component.h"
#include "Utilities/include/SimpleSprite.h"
#include <memory>
using namespace std;

class Renderable: public Component{
public:
    Renderable(CSimpleSprite* newSprite = nullptr) : m_sprite(newSprite) {}

    CSimpleSprite* GetSprite() const { return m_sprite; }
    void SetSprite(CSimpleSprite* newSprite) { m_sprite = newSprite; }

private:
    CSimpleSprite* m_sprite = nullptr;
};
