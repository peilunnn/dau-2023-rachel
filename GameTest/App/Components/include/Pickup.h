#pragma once
#include "Components/include/Component.h"
#include "Utilities/include/Enums.h"

class Pickup : public Component
{
public:
    Pickup(PickupType type) : pickupType(type) {}

    PickupType GetPickupType() const { return pickupType; }

private:
    PickupType pickupType;
};

