#pragma once
#include "Components/include/Component.h"

class Screen : public Component {
public:
    static Screen& GetInstance() {
        static Screen instance;
        return instance;
    }
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

    const float SCREEN_LEFT = NDCtoScreenX(BORDER_LEFT_X, SCREEN_WIDTH);
    const float SCREEN_RIGHT = NDCtoScreenX(BORDER_RIGHT_X, SCREEN_WIDTH);
    const float SCREEN_TOP = NDCtoScreenY(BORDER_TOP_Y, SCREEN_HEIGHT);
    const float SCREEN_BOTTOM = NDCtoScreenY(BORDER_BOTTOM_Y, SCREEN_HEIGHT);

    const float TITLE_X_OFFSET = 520.0f;
    const float TITLE_Y_OFFSET = 300.0f;
    const float DESCRIPTION_X_OFFSET = 600.0f;
    const float DESCRIPTION_Y_OFFSET = 450.0f;
    const float PLAY_BUTTON_X_OFFSET = 520.0f;
    const float PLAY_BUTTON_Y_OFFSET = 500.0f;

    const float R_MAIN_MENU_BG = 0.0f;
    const float G_MAIN_MENU_BG = 0.0f;
    const float B_MAIN_MENU_BG = 0.0f;
    const float ALPHA_MAIN_MENU_BG = 1.0f;

    const float R_GAMEPLAY_BG = 0.2f;
    const float G_GAMEPLAY_BG = 0.2f;
    const float B_GAMEPLAY_BG = 0.2f;
    const float ALPHA_GAMEPLAY_BG = 1.0f;

    const float R_BORDER = 1.0f;
    const float G_BORDER = 1.0f;
    const float B_BORDER = 1.0f;

    const float R_BG_IN_BORDER = 0.0f;
    const float G_BG_IN_BORDER = 0.0f;
    const float B_BG_IN_BORDER = 0.0f;

    const float R_TEXT = 1.0f;
    const float G_TEXT = 1.0f;
    const float B_TEXT = 1.0f;

    float Screen::NDCtoScreenX(float ndcX, float screenWidth) {
        return (ndcX + 1.0f) * 0.5f * screenWidth;
    }

    float Screen::NDCtoScreenY(float ndcY, float screenHeight) {
        return (1.0f - ndcY) * 0.5f * screenHeight;
    }

private:
    Screen() = default;
};
