
#include <iostream>
#include <string>

#include "debug.hpp"
#include "unlock_map.hpp"

static constexpr auto src = "UnlockMap:TEST";
using namespace std;
using namespace Dattatypes;

#define LOG_CONTENT \
    LOG_DEBUG("Content:"); \
    for (auto elem : colors._data) { \
        LOG_DEBUG("* {}", elem); \
    }



// Testing
int main() {
    LOG_INFO("=== Beginning Tests for Unlock Map ===");

enum class Number : int8_t {
    N_18 = -18,
    N_17, N_16, N_15, N_14, N_13, N_12,
    N_11, N_10, N_9, N_8, N_7, N_6,
    N_5, N_4, N_3, N_2, N_1, ZERO,
    P_1, P_2, P_3, P_4, P_5, P_6,
    P_7, P_8, P_9, P_10, P_11, P_12,
    P_13, P_14, P_15, P_16, P_17, P_18
};



    std::string test;
    int num=0;
    unlock_map<Number> colors;

    test = "Empty check";
    LOG_WARN("Test {} : {}", ++num, test);
    runtime_assert(colors.empty(), test, 1);

    test = "Single insert into empty"; // +2
    LOG_WARN("Test {} : {}", ++num, test);
    colors.insert(Number::ZERO);
    runtime_assert(!colors.check(Number::N_1), test, 1);
    runtime_assert(colors.check(Number::ZERO), test, 2);
    runtime_assert(!colors.check(Number::P_1), test, 3);
    runtime_assert(!(colors._data.size() & 1), test, "sanity");
    runtime_assert(colors._data.size() == 2, test, "datasize");
    runtime_assert(colors.size() == 1, test, "size");

    test = "Range insert across single |"; // mod2
    LOG_WARN("Test {} : {}", ++num, test);
    colors.insert(Number::N_2, Number::P_2);
    runtime_assert(!colors.check(Number::N_3), test, 1);
    runtime_assert(colors.check(Number::N_2), test, 2);
    runtime_assert(colors.check(Number::ZERO), test, 3);
    runtime_assert(colors.check(Number::P_2), test, 4);
    runtime_assert(!colors.check(Number::P_3), test, 5);
    runtime_assert(!(colors._data.size() & 1), test, "sanity");
    runtime_assert(colors._data.size() == 2, test, "datasize");
    runtime_assert(colors.size() == 5, test, ("size " + std::to_string(colors.size())));
    LOG_CONTENT

    test = "Range insert across upper"; // mod1
    LOG_WARN("Test {} : {}", ++num, test);
    colors.insert(Number::N_4, Number::ZERO);
    runtime_assert(!colors.check(Number::N_5), test, 1);
    runtime_assert(colors.check(Number::N_4), test, 2);
    runtime_assert(colors.check(Number::P_2), test, 3);
    runtime_assert(!(colors._data.size() & 1), test, "sanity");
    runtime_assert(colors._data.size() == 2, test, "datasize");
    runtime_assert(colors.size() == 7, test, ("size " + std::to_string(colors.size())));
    LOG_CONTENT

    test = "Range insert across lower"; // mod1
    LOG_WARN("Test {} : {}", ++num, test);
    colors.insert(Number::ZERO, Number::P_4);
    runtime_assert(colors.check(Number::N_4), test, 1);
    runtime_assert(colors.check(Number::P_4), test, 2);
    runtime_assert(!colors.check(Number::P_5), test, 3);
    runtime_assert(!(colors._data.size() & 1), test, "sanity");
    runtime_assert(colors._data.size() == 2, test, "datasize");
    runtime_assert(colors.size() == 9, test, ("size " + std::to_string(colors.size())));
    LOG_CONTENT

    test = "Single erase into empty"; // nop
    LOG_WARN("Test {} : {}", ++num, test);
    colors.erase(Number::P_15);
    runtime_assert(!(colors._data.size() & 1), test, "sanity");
    runtime_assert(colors._data.size() == 2, test, "datasize");
    runtime_assert(colors.size() == 9, test, ("size " + std::to_string(colors.size())));
    LOG_CONTENT

    test = "Single insert into range"; // nop
    LOG_WARN("Test {} : {}", ++num, test);
    colors.insert(Number::ZERO);
    runtime_assert(!(colors._data.size() & 1), test, "sanity");
    runtime_assert(colors._data.size() == 2, test, "datasize");
    runtime_assert(colors.size() == 9, test, ("size " + std::to_string(colors.size())));
    LOG_CONTENT

    test = "Single erase into range"; // +2
    LOG_WARN("Test {} : {}", ++num, test);
    colors.erase(Number::ZERO);
    runtime_assert(!(colors._data.size() & 1), test, "sanity");
    runtime_assert(colors._data.size() == 4, test, "datasize");
    runtime_assert(colors.size() == 8, test, ("size " + std::to_string(colors.size())));
    LOG_CONTENT

    test = "Single insert into empty"; // -2
    LOG_WARN("Test {} : {}", ++num, test);
    colors.insert(Number::ZERO);
    runtime_assert(!(colors._data.size() & 1), test, "sanity");
    runtime_assert(colors._data.size() == 2, test, "datasize");
    runtime_assert(colors.size() == 9, test, ("size " + std::to_string(colors.size())));
    LOG_CONTENT

    // test = "Range erase inside range"; // +2
    // LOG_WARN("Test {} : {}", ++num, test);
    // colors.erase(Number::N_2, Number::P_2);
    // runtime_assert(!colors.check(Number::N_3), test, 1);
    // runtime_assert(colors.check(Number::N_2), test, 2);
    // runtime_assert(colors.check(Number::P_2), test, 3);
    // runtime_assert(!colors.check(Number::P_3), test, 4);
    // runtime_assert(!(colors._data.size() & 1), test, "sanity");
    // runtime_assert(colors._data.size() == 4, test, "datasize");
    // runtime_assert(colors.size() == 4, test, ("size " + std::to_string(colors.size())));
    // LOG_CONTENT

    // test = "Range insert inside gap"; // -2
    // LOG_WARN("Test {} : {}", ++num, test);
    // colors.insert(Number::N_1, Number::P_1);
    // runtime_assert(!colors.check(Number::N_5), test, 1);
    // runtime_assert(colors.check(Number::N_4), test, 2);
    // runtime_assert(colors.check(Number::P_4), test, 3);
    // runtime_assert(!colors.check(Number::P_5), test, 4);
    // runtime_assert(!(colors._data.size() & 1), test, "sanity");
    // runtime_assert(colors._data.size() == 2, test, "datasize");
    // runtime_assert(colors.size() == 9, test, ("size " + std::to_string(colors.size())));
    // LOG_CONTENT

    test = "Range erase across upper"; // mod1
    test = "Range erase across lower"; // mod1
    test = "Single erase at upper"; // mod1
    test = "Single erase at lower"; // mod1
    test = "Single insert onto upper"; // mod1
    test = "Single insert onto lower"; // mod1




    LOG_INFO("=== All tests for Unlock Map passed! ===\n\n");
    return 0;
}
