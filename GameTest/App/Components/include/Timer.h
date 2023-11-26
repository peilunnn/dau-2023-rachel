#pragma once
#include "Component.h"

struct Timer : public Component {
    float countdownTime;

    Timer(float startTime) : countdownTime(startTime) {}
};