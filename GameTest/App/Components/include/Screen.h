#pragma once
#include "Components/include/Component.h"

class Screen : public Component {
public:
    static Screen& GetInstance();
    Screen(Screen const&) = delete;
    void operator=(Screen const&) = delete;

    const float SCREEN_WIDTH = 1024.0f;
    const float SCREEN_HEIGHT = 768.0f;
    const float BORDER_LEFT_X = -0.9f;
    const float BORDER_TOP_Y = 0.75f;
    const float BORDER_RIGHT_X = 0.9f;
    const float BORDER_BOTTOM_Y = -0.7f;
    const float BORDER_THICKNESS = 10.0f;
    const int AMMO_SPRITE_SPACING = 30;

    const float BORDER_LEFT_SCREEN_COORDS = NDCtoScreenX(BORDER_LEFT_X, SCREEN_WIDTH);
    const float BORDER_RIGHT_SCREEN_COORDS = NDCtoScreenX(BORDER_RIGHT_X, SCREEN_WIDTH);
    const float BORDER_TOP_SCREEN_COORDS = NDCtoScreenY(BORDER_TOP_Y, SCREEN_HEIGHT);
    const float BORDER_BOTTOM_SCREEN_COORDS = NDCtoScreenY(BORDER_BOTTOM_Y, SCREEN_HEIGHT);

    float Screen::NDCtoScreenX(float ndcX, float screenWidth) {
        return (ndcX + 1.0f) * 0.5f * screenWidth;
    }

    float Screen::NDCtoScreenY(float ndcY, float screenHeight) {
        return (1.0f - ndcY) * 0.5f * screenHeight;
    }

private:
    Screen() = default;
};
