#pragma once
#include <cstdio>
#include <cstdarg>
#include <mutex>


class SimpleLogger
{
public:
    SimpleLogger() {

    }
    enum color {def, red, green, yellow, blue, num};
    void printf(const char* format, ...);
    void printf(color c, const char* format, ...);
private:
    const char* color_defs[color::num] = { "", "\033[31m", "\033[32m", "\033[33m", "\033[34m"};
    
    static std::mutex _mtx;
};

