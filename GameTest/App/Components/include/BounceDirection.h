#pragma once
#include "Components/include/Component.h"

class BounceDirection : public Component {
public:
    BounceDirection() = default;

    bool GetBounced() const { return m_bounced; }
    void SetBounced(bool newBounced) { m_bounced = newBounced; }

private:
    bool m_bounced = false;
};