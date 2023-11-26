#include "stdafx.h"
#include "../include/ScreenHandler.h"

const float ScreenHandler::SCREEN_LEFT = ScreenHandler::NDCtoScreenX(ScreenHandler::BORDER_LEFT_X, ScreenHandler::SCREEN_WIDTH);
const float ScreenHandler::SCREEN_RIGHT = ScreenHandler::NDCtoScreenX(ScreenHandler::BORDER_RIGHT_X, ScreenHandler::SCREEN_WIDTH);
const float ScreenHandler::SCREEN_TOP = ScreenHandler::NDCtoScreenY(ScreenHandler::BORDER_TOP_Y, ScreenHandler::SCREEN_HEIGHT);
const float ScreenHandler::SCREEN_BOTTOM = ScreenHandler::NDCtoScreenY(ScreenHandler::BORDER_BOTTOM_Y, ScreenHandler::SCREEN_HEIGHT);
