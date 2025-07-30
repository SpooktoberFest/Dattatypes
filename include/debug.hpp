#pragma once

#include <iostream>
#include <format>


#define LOG_LEVEL 5
/*
    0 : No logging
    1 : Error
    2 : Warn
    3 : Info
    4 : Debug
*/


#if LOG_LEVEL >= 5
#define LOG_DEBUG(...) \
    std::cout << "DEBUG: [" << src << "] " << std::format(__VA_ARGS__) << std::endl;
#else
#define LOG_DEBUG(...)
#endif

#if LOG_LEVEL >= 4
#define LOG_INFO(...) \
    std::cout << "INFO: [" << src << "] " << std::format(__VA_ARGS__) << std::endl;
#else
#define LOG_INFO(...)
#endif

#if LOG_LEVEL >= 3
#define LOG_WARN(...) \
    std::cerr << "WARN: [" << src << "] " << std::format(__VA_ARGS__) << std::endl;
#else
#define LOG_WARN(...)
#endif

#if LOG_LEVEL >= 2
#define LOG_ERROR(...) \
    std::cerr << "ERROR: [" << src << "] " << std::format(__VA_ARGS__) << std::endl;
#else
#define LOG_ERROR(...)
#endif

#if LOG_LEVEL >= 1
#define LOG_FATAL(...) \
    std::cerr << "FATAL: [" << src << "] " << std::format(__VA_ARGS__) << std::endl;
#else
#define LOG_FATAL(...)
#endif
