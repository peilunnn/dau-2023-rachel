#include "stdafx.h"
#include "Components/include/Screen.h"#include "Screen.h"

Screen &Screen::GetInstance()
{
    static Screen instance;
    return instance;
}
