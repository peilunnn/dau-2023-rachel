#pragma once
#include "Components/include/Component.h"
#include "Utilities/include/Enums.h"

class Pickup : public Component
{
public:
    Pickup(PickupType type) : m_pickupType(type) {}

    PickupType GetPickupType() const { return m_pickupType; }

private:
    PickupType m_pickupType = PickupType::None;
};

