#pragma once
#include "Component.h"

struct Timer : public Component {
    float countdownTime = 60;

    Timer() = default;
};