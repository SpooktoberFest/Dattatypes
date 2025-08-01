#pragma once
// === HEADER ONLY ===

#include <iostream>
#include <format>

#define LOG_LEVEL 5
/*
    0 : No logging
    1 : Fatal
    2 : Error
    3 : Warn
    4 : Info
    5 : Debug
*/

#define ANSI_COLOR_RESET   "\033[0m"
#define ANSI_COLOR_RED     "\033[31m"
#define ANSI_COLOR_GREEN   "\033[32m"
#define ANSI_COLOR_YELLOW  "\033[33m"
#define ANSI_COLOR_MAGENTA "\033[35m"
#define ANSI_COLOR_CYAN    "\033[36m"

#if LOG_LEVEL >= 5
#define LOG_DEBUG(...) \
    std::cout << ANSI_COLOR_CYAN << "DEBUG: [" << src << "] " << std::format(__VA_ARGS__) << ANSI_COLOR_RESET << std::endl;
#else
#define LOG_DEBUG(...)
#endif

#if LOG_LEVEL >= 4
#define LOG_INFO(...) \
    std::cout << "INFO: [" << src << "] " << std::format(__VA_ARGS__) << ANSI_COLOR_RESET << std::endl;
#else
#define LOG_INFO(...)
#endif

#if LOG_LEVEL >= 3
#define LOG_WARN(...) \
    std::cerr << ANSI_COLOR_YELLOW << "WARN: [" << src << "] " << std::format(__VA_ARGS__) << ANSI_COLOR_RESET << std::endl;
#else
#define LOG_WARN(...)
#endif

#if LOG_LEVEL >= 2
#define LOG_ERROR(...) \
    std::cerr << ANSI_COLOR_RED << "ERROR: [" << src << "] " << std::format(__VA_ARGS__) << ANSI_COLOR_RESET << std::endl;
#else
#define LOG_ERROR(...)
#endif

#if LOG_LEVEL >= 1
#define LOG_FATAL(...) \
    std::cerr << ANSI_COLOR_MAGENTA << "FATAL: [" << src << "] " << std::format(__VA_ARGS__) << ANSI_COLOR_RESET << std::endl;
#else
#define LOG_FATAL(...)
#endif

#define runtime_assert(pred, test, num) \
    if (pred) { LOG_INFO("{}Assertion success: {} {}", ANSI_COLOR_GREEN, test, num); } \
    else { LOG_ERROR("Assertion failure: {} {}", test, num); }
