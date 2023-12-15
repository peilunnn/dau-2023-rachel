//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#ifndef _MAIN_H_
#define _MAIN_H_

#include <Windows.h>

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern HWND MAIN_WINDOW_HANDLE;

class CProfiler
{
public:
    CProfiler();
    void Start();
    double Stop();
    void Print(float x, float y, const char* text);

private:
    double m_startTime;
    double m_elapsedTime;
};

#endif