#pragma once
#include "Component.h"

struct Score : public Component {
    int score;
    Score(int initialScore = 0) : score(initialScore) {}
};