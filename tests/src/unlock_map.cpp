
#include <iostream>
#include <string>

#include "debug.hpp"
#include "unlock_map.hpp"

static constexpr auto src = "UnlockMap:TEST";
using namespace std;
using namespace Dattatypes;


// Testing
int main() {
    LOG_INFO("=== Beginning Tests for Unlock Map ===");

    enum class Color {
        RED=0,
        GREEN,
        BLUE,
        CYAN,
        YELLOW,
        MAGENTA,
        BLACK,
        WHITE,
        GRAY,
        DARK_RED,
        DARK_GREEN,
        DARK_BLUE,
        DARK_CYAN,
        DARK_YELLOW, //
        DARK_MAGENTA,
        DARK_BLACK,
        DARK_WHITE,
        DARK_GRAY,
        LIGHT_RED,
        LIGHT_GREEN,
        LIGHT_BLUE,
        LIGHT_CYAN,
        LIGHT_YELLOW,
        LIGHT_MAGENTA,
        LIGHT_BLACK,
        LIGHT_WHITE,
        LIGHT_GRAY
    };

    std::string test;
    int num;
    unlock_map<Color> colors;

    test = "Empty check";
    runtime_assert(colors.empty(), test, 1);

    // test = "Single insert into empty"; // +2
    // colors.insert(Color::DARK_YELLOW);
    // runtime_assert(!colors.check(Color::DARK_CYAN), test, 1);
    // runtime_assert(colors.check(Color::DARK_YELLOW), test, 2);
    // runtime_assert(!colors.check(Color::DARK_MAGENTA), test, 3);
    // runtime_assert(!(colors._data.size() & 1), test, "sanity");
    // runtime_assert(colors._data.size() == 2, test, "datasize");
    // runtime_assert(colors.size() == 1, test, "size");

    test = "Range insert across single |"; // mod2
    colors.insert(Color::DARK_CYAN, Color::DARK_MAGENTA);
    runtime_assert(!colors.check(Color::DARK_BLUE), test, 1);
    runtime_assert(colors.check(Color::DARK_CYAN), test, 2);
    runtime_assert(colors.check(Color::DARK_YELLOW), test, 5);
    runtime_assert(colors.check(Color::DARK_MAGENTA), test, 3);
    runtime_assert(!colors.check(Color::DARK_BLACK), test, 4);
    runtime_assert(!(colors._data.size() & 1), test, "sanity");
    runtime_assert(colors._data.size() == 2, test, "datasize");
    runtime_assert(colors.size() == 3, test, "size");
    LOG_DEBUG("Content:");
    for (auto elem : colors._data) {
        LOG_DEBUG("* {}", elem);
    }


    test = "Range insert across upper"; // mod1
    test = "Range insert across lower"; // mod1
    test = "Single erase into empty"; // nop
    test = "Single insert into range"; // nop
    test = "Range erase inside range"; // +2
    test = "Range insert onto gap"; // -2
    test = "Range erase across upper"; // mod1
    test = "Range erase across lower"; // mod1
    test = "Single erase at upper"; // mod1
    test = "Single erase at lower"; // mod1
    test = "Single insert onto upper"; // mod1
    test = "Single insert onto lower"; // mod1




    LOG_INFO("=== All tests for Unlock Map passed! ===\n\n");
    return 0;
}
