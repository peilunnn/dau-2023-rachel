#pragma once
#include "Component.h"

class Score : public Component {
public:
    Score() = default;

    int GetScore() const { return m_title; }
    void SetScore(int newScore) { m_title = newScore; }

private:
    int m_title = 0;
};