#pragma once

#include <chrono>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>

#if WIN32 || _WIN32 || __WIN32__ || __NT__
#include <windows.h>
#endif

namespace Debug {

#if __APPLE__ || __linux__

    enum TextColor { WHITE = 0, RED = 31, GREEN = 32, YELLOW = 33, BLUE = 34, PURPLE = 35, CYAN = 36 };

#else

    enum TextColor { WHITE = 0x0F, RED = 0x0C, GREEN = 0x0A, YELLOW = 0x0E, BLUE = 0x09, PURPLE = 0x0D, CYAN = 0x0B };

#endif

    enum LogLevel { NONE = 0, FATAL = 1, ERR = 2, WARNING = 4, INFO = 8, ALL = 15 };

    inline unsigned char logLevel = ALL;
    inline bool showTimestamp = true;

    inline void printWithColor(const TextColor color, const std::string& message) {
#if __APPLE__ || __linux__
        printf("\033[0;%dm%s\033[0m\n", color, message.c_str());
#else
        HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(outputHandle, color);
        printf("%s\n", message.c_str());
        SetConsoleTextAttribute(outputHandle, TextColor::WHITE);
#endif
    }

    template<typename... Args>
    void log(const unsigned char level, const Args... args) {
        // Compare with logLevel Bitmask to see if level is included, return if not
        if (~logLevel & level)
            return;

        std::ostringstream stream;

        if (showTimestamp) {
            const auto now = std::chrono::system_clock::now();
            const auto currentTime = std::chrono::system_clock::to_time_t(now);

            std::string currentTimeString = ctime(&currentTime);
            currentTimeString.pop_back();
            stream << '[' << currentTimeString << "] ";
        }

        std::string logLevelString;
        TextColor color;

        switch (level) {
            case FATAL:
                logLevelString = "[FATAL] ";
                color = RED;
                break;
            case ERR:
                logLevelString = "[ERROR] ";
                color = RED;
                break;
            case WARNING:
                logLevelString = "[WARN]  ";
                color = YELLOW;
                break;
            case INFO:
                logLevelString = "[INFO]  ";
                color = BLUE;
                break;
            default:
                log(ERR, "Invalid log level specified");
                return;
        }

        stream << logLevelString;
        (stream << ... << args);
        printWithColor(color, stream.str());
    }
}  // namespace Debug
