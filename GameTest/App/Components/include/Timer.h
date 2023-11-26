#ifndef TIMER_H
#define TIMER_H

#include "Component.h"

struct Timer : public Component {
    float countdownTime;

    Timer(float startTime) : countdownTime(startTime) {}
};

#endif // TIMER_H
