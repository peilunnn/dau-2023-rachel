#pragma once
#include "Components/include/Component.h"
#include "Utilities/include/Enums.h"

class Ammo : public Component
{
public:
    Ammo(AmmoType type) : m_ammoType(type) {}

    AmmoType GetAmmoType() const { return m_ammoType; }

private:
    AmmoType m_ammoType;
};

