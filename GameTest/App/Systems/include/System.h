#pragma once
#include "../App/Utilities/include/Enums.h"

class System
{
public:
    virtual ~System() = default;
    virtual SystemType GetSystemType() const = 0;
};