#pragma once
#include "Component.h"

class Direction : public Component {
public:
    Direction() = default;

    bool GetBounced() const { return m_bounced; }
    void SetBounced(bool newBounced) { m_bounced = newBounced; }

private:
    bool m_bounced = false;
};