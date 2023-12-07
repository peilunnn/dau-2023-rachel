#pragma once
#include "System.h"

class TitleHandler : public System {
public:
    static TitleHandler& GetInstance() {
        static TitleHandler instance;
        return instance;
    }
    TitleHandler(TitleHandler const&) = delete;
    void operator=(TitleHandler const&) = delete;

    void OscillateTitle(float deltaTime);

private:
    TitleHandler() = default;

    float m_accumulatedTime = 0.0f;
};