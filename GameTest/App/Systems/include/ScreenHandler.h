#ifndef SCREEN_HANDLER_H
#define SCREEN_HANDLER_H

class ScreenHandler
{
public:
    static constexpr float SCREEN_WIDTH = 1024.0f;
    static constexpr float SCREEN_HEIGHT = 768.0f;
    static constexpr float BORDER_LEFT_X = -0.9f;
    static constexpr float BORDER_TOP_Y = 0.75f;
    static constexpr float BORDER_RIGHT_X = 0.9f;
    static constexpr float BORDER_BOTTOM_Y = -0.7f;
    static constexpr float BORDER_THICKNESS = 10.0f;
    static constexpr int AMMO_SPRITE_SPACING = 30;

    static const float SCREEN_LEFT;
    static const float SCREEN_RIGHT;
    static const float SCREEN_TOP;
    static const float SCREEN_BOTTOM;

    // For converting normalized device coordinates to screen coordinates
    static float NDCtoScreenX(float ndcX, float screenWidth)
    {
        return (ndcX + 1.0f) * 0.5f * screenWidth;
    }

    static float NDCtoScreenY(float ndcY, float screenHeight)
    {
        return (1.0f - ndcY) * 0.5f * screenHeight;
    }
};

#endif // SCREEN_HANDLER_H