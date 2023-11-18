#ifndef DEBUG_H
#define DEBUG_H

#include <Windows.h>
#include <string>
#include <iostream>
#include <sstream>

class Debug {
public:
    template<typename T>
    static void Log(const std::string& message, T value) {
        std::stringstream ss;
        ss << message << value << "\n";
        OutputDebugStringA(ss.str().c_str());
    }

    static void Log(const std::string& message) {
        OutputDebugStringA((message + "\n").c_str());
    }
};

#endif // DEBUG_H
