#pragma once
#include "Components/include/Component.h"

class Score : public Component 
{
public:
    Score() = default;

    int GetScore() const { return m_score; }
    void SetScore(int newScore) { m_score = newScore; }

private:
    int m_score = 0;
};