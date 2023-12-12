#pragma once

struct Color
{
    Color(float red, float green, float blue, float a = 1.0f)
        : r(red), g(green), b(blue), alpha(a) {}

    float r, g, b, alpha;
};

const Color BLACK(0.0f, 0.0f, 0.0f);
const Color DARK_GREY(0.2f, 0.2f, 0.2f);
const Color WHITE(1.0f, 1.0f, 1.0f);