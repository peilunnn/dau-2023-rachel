#pragma once
#include "Components/include/Component.h"

class Screen : public Component {
public:
    static Screen& GetInstance();
    Screen(Screen const&) = delete;
    void operator=(Screen const&) = delete;

    const float SCREEN_WIDTH = 1024.0f;
    const float SCREEN_HEIGHT = 768.0f;
    const float SCREEN_LEFT_NDC = -1.0f;
    const float SCREEN_RIGHT_NDC = 1.0f;
    const float SCREEN_BOTTOM_NDC = -1.0f;
    const float SCREEN_TOP_NDC = 1.0f;
    const float BORDER_LEFT_NDC = -0.9f;
    const float BORDER_TOP_NDC = 0.75f;
    const float BORDER_RIGHT_NDC = 0.9f;
    const float BORDER_BOTTOM_NDC = -0.7f;

    const float BORDER_THICKNESS = 10.0f;
    const int AMMO_SPRITE_SPACING = 30;

    const float BORDER_LEFT_SCREEN_COORD = NDCtoScreenX(BORDER_LEFT_NDC, SCREEN_WIDTH);
    const float BORDER_RIGHT_SCREEN_COORD = NDCtoScreenX(BORDER_RIGHT_NDC, SCREEN_WIDTH);
    const float BORDER_TOP_SCREEN_COORD = NDCtoScreenY(BORDER_TOP_NDC, SCREEN_HEIGHT);
    const float BORDER_BOTTOM_SCREEN_COORD = NDCtoScreenY(BORDER_BOTTOM_NDC, SCREEN_HEIGHT);

    float Screen::NDCtoScreenX(float ndcX, float screenWidth) {
        return (ndcX + 1.0f) * 0.5f * screenWidth;
    }

    float Screen::NDCtoScreenY(float ndcY, float screenHeight) {
        return (1.0f - ndcY) * 0.5f * screenHeight;
    }

private:
    Screen() = default;
};
