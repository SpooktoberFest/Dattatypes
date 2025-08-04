#pragma once
// === HEADER ONLY ===
// Silently pulls `src`

#include <iostream>
#include <format>

#ifndef LOG_LEVEL
#define LOG_LEVEL 5
#endif

#define ANSI_COLOR_RESET   "\033[0m"
#define ANSI_COLOR_RED     "\033[31m"
#define ANSI_COLOR_GREEN   "\033[32m"
#define ANSI_COLOR_YELLOW  "\033[33m"
#define ANSI_COLOR_MAGENTA "\033[35m"
#define ANSI_COLOR_CYAN    "\033[36m"

#define LOG_HEADING(severity) severity << ": [" << src << ":" << __LINE__ << "] "

#if LOG_LEVEL >= 5
#define LOG_DEBUG(...) \
    std::cout << ANSI_COLOR_CYAN << LOG_HEADING("DEBUG") << std::format(__VA_ARGS__) << ANSI_COLOR_RESET << std::endl;
#else
#define LOG_DEBUG(...)
#endif

#if LOG_LEVEL >= 4
#define LOG_INFO(...) \
    std::cout << LOG_HEADING("INFO") << std::format(__VA_ARGS__) << ANSI_COLOR_RESET << std::endl;
#else
#define LOG_INFO(...)
#endif

#if LOG_LEVEL >= 3
#define LOG_WARN(...) \
    std::cout << ANSI_COLOR_YELLOW << LOG_HEADING("WARN") << std::format(__VA_ARGS__) << ANSI_COLOR_RESET << std::endl;
#else
#define LOG_WARN(...)
#endif

#if LOG_LEVEL >= 2
#define LOG_ERROR(...) \
    std::cerr << ANSI_COLOR_RED << LOG_HEADING("ERROR") << std::format(__VA_ARGS__) << ANSI_COLOR_RESET << std::endl;
#else
#define LOG_ERROR(...)
#endif

#if LOG_LEVEL >= 1
#define LOG_FATAL(...) \
    std::cerr << ANSI_COLOR_MAGENTA << LOG_HEADING("FATAL") << std::format(__VA_ARGS__) << ANSI_COLOR_RESET << std::endl;
#else
#define LOG_FATAL(...)
#endif

#define runtime_assert(expression, expected, name) \
    if ((expression) == expected) { LOG_INFO("{}Assertion success: {} = {}", ANSI_COLOR_GREEN, name, expected); } \
    else { LOG_ERROR("Assertion failure: {} = {} (expected {})", name, expression, expected); }
