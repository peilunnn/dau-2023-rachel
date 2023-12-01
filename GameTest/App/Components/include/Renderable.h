#pragma once
#include "Utilities/include/SimpleSprite.h"
#include "Component.h"
#include <memory>
using namespace std;

struct Renderable : public Component {
    Renderable(shared_ptr<CSimpleSprite> newSprite) : m_sprite(newSprite) {}

    shared_ptr<CSimpleSprite> GetSprite() const { return m_sprite; }
    void SetSprite(shared_ptr<CSimpleSprite> newSprite) { m_sprite = newSprite; }

private:
    shared_ptr<CSimpleSprite> m_sprite = nullptr;
};
