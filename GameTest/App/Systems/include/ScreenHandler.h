#pragma once
#include "Systems/include/System.h"

class ScreenHandler : public System {
public:
    static ScreenHandler& GetInstance() {
        static ScreenHandler instance;
        return instance;
    }
    ScreenHandler(ScreenHandler const&) = delete;
    void operator=(ScreenHandler const&) = delete;
    
    const float SCREEN_WIDTH = 1024.0f;
    const float SCREEN_HEIGHT = 768.0f;
    const float BORDER_LEFT_X = -0.9f;
    const float BORDER_TOP_Y = 0.75f;
    const float BORDER_RIGHT_X = 0.9f;
    const float BORDER_BOTTOM_Y = -0.7f;
    const float BORDER_THICKNESS = 10.0f;
    const int AMMO_SPRITE_SPACING = 30;

    const float SCREEN_LEFT = NDCtoScreenX(BORDER_LEFT_X, SCREEN_WIDTH);
    const float SCREEN_RIGHT = NDCtoScreenX(BORDER_RIGHT_X, SCREEN_WIDTH);
    const float SCREEN_TOP = NDCtoScreenY(BORDER_TOP_Y, SCREEN_HEIGHT);
    const float SCREEN_BOTTOM = NDCtoScreenY(BORDER_BOTTOM_Y, SCREEN_HEIGHT);

    float NDCtoScreenX(float ndcX, float screenWidth)
    {
        return (ndcX + 1.0f) * 0.5f * screenWidth;
    }

    float NDCtoScreenY(float ndcY, float screenHeight)
    {
        return (1.0f - ndcY) * 0.5f * screenHeight;
    }

private:
    ScreenHandler() = default;
};