#ifndef SCORE_H
#define SCORE_H

#include "Component.h"

struct Score : public Component {
    int score;
    Score(int initialScore = 0) : score(initialScore) {}
};

#endif // SCORE_H
