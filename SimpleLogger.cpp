#include "SimpleLogger.h"

std::mutex SimpleLogger::_mtx;

void SimpleLogger::printf(const char* format, ...)
{
    std::lock_guard<std::mutex> lock(_mtx);
    va_list args;
    va_start(args, format); // Initialize args to retrieve arguments after 'format'
    std::vprintf(format, args); // Forward the va_list to vprintf
    va_end(args); // Clean up the va_list
    std::printf("\n");
}

void SimpleLogger::printf(color c, const char* format, ...)
{
    std::lock_guard<std::mutex> lock(_mtx);
    if (c) {
        std::printf(color_defs[c]);
    }
    va_list args;
    va_start(args, format); // Initialize args to retrieve arguments after 'format'
    std::vprintf(format, args); // Forward the va_list to vprintf
    va_end(args); // Clean up the va_list
    if (c) {
        std::printf("\033[0m");
    }
    std::printf("\n");
}
