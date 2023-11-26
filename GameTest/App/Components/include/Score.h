#pragma once
#include "Component.h"

struct Score : public Component {
    int score = 0;

    Score() = default;
};