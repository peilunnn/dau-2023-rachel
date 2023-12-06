#pragma once
#include "Component.h"

struct Score : public Component {
    Score() = default;

    int GetScore() const { return m_title; }
    void SetScore(int newScore) { m_title = newScore; }

private:
    int m_title = 0;
};