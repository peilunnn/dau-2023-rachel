#pragma once
#include "Component.h"

struct Score : public Component {
    Score() = default;

    int GetScore() const { return m_score; }
    void SetScore(int newScore) { m_score = newScore; }

private:
    int m_score = 0;
};