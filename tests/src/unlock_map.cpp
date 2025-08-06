
#include <iostream>
#include <string>

#include "debug.hpp"
#include "unlock_map.hpp"
#include "string"

static constexpr auto src = "unlock_map:TEST";
using namespace std;
using namespace dattatypes;


enum class Number : int8_t {
    N_18 = -18,
    N_17, N_16, N_15, N_14, N_13, N_12,
    N_11, N_10, N_9, N_8, N_7, N_6,
    N_5, N_4, N_3, N_2, N_1, ZERO,
    P_1, P_2, P_3, P_4, P_5, P_6,
    P_7, P_8, P_9, P_10, P_11, P_12,
    P_13, P_14, P_15, P_16, P_17, P_18
};

std::string vec2str(const std::vector<int8_t>& vec) {
    std::string result = "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        result += std::to_string(int(vec[i]));
        if (i != vec.size() - 1)
            result += ", ";
    }
    result += "]";
    return result;
}


// Testing
int main() {
    LOG_INFO("=== Beginning Tests for unlock_map ===");

    int num=0;
    unlock_map<Number> map;

    LOG_WARN("Test {} - Empty check", ++num);
    runtime_assert(map.empty(), 1, "empty()");

    LOG_WARN("Test {} - Single insert into empty", ++num);
    map.insert(Number::ZERO);
    runtime_assert(map.check(Number::N_1), false, "check(N_1)");
    runtime_assert(map.check(Number::ZERO), true, "check(ZERO)");
    runtime_assert(map.check(Number::P_1), false, "check(P_1)");
    runtime_assert(!(map._data.size()&1), true, "sanity");
    runtime_assert(map._data.size(), 2, "datasize");
    runtime_assert(map.size(), 1, "size");

    LOG_WARN("Test {} - Range insert across single", ++num);
    map.insert(Number::N_2, Number::P_2);
    runtime_assert(map.check(Number::N_3), false, "check(N_3)");
    runtime_assert(map.check(Number::N_2), true, "check(N_2)");
    runtime_assert(map.check(Number::ZERO), true, "check(ZERO)");
    runtime_assert(map.check(Number::P_2), true, "check(P_2)");
    runtime_assert(map.check(Number::P_3), false, "check(P_3)");
    runtime_assert(!(map._data.size()&1), true, "sanity");
    runtime_assert(map._data.size(), 2, "datasize");
    runtime_assert(map.size(), 5, "size");

    LOG_WARN("Test {} - Range insert across lower", ++num);
    map.insert(Number::N_4, Number::ZERO);
    runtime_assert(vec2str(map._data), "[-4, 3]", "data");
    runtime_assert(!(map._data.size()&1), true, "sanity");
    runtime_assert(map._data.size(), 2, "datasize");
    runtime_assert(map.size(), 7, "size");

    LOG_WARN("Test {} - Range insert across upper", ++num);
    map.insert(Number::ZERO, Number::P_4);
    runtime_assert(vec2str(map._data), "[-4, 5]", "data");
    runtime_assert(!(map._data.size()&1), true, "sanity");
    runtime_assert(map._data.size(), 2, "datasize");
    runtime_assert(map.size(), 9, "size");

    LOG_WARN("Test {} - Single erase into empty", ++num);
    map.erase(Number::P_15);
    runtime_assert(vec2str(map._data), "[-4, 5]", "data");
    runtime_assert(!(map._data.size()&1), true, "sanity");
    runtime_assert(map._data.size(), 2, "datasize");
    runtime_assert(map.size(), 9, "size");

    LOG_WARN("Test {} - Single insert into range", ++num);
    map.insert(Number::ZERO);
    runtime_assert(vec2str(map._data), "[-4, 5]", "data");
    runtime_assert(!(map._data.size()&1), true, "sanity");
    runtime_assert(map._data.size(), 2, "datasize");
    runtime_assert(map.size(), 9, "size");

    LOG_WARN("Test {} - Single erase into range", ++num);
    map.erase(Number::ZERO);
    runtime_assert(vec2str(map._data), "[-4, 0, 1, 5]", "data");
    runtime_assert(!(map._data.size()&1), true, "sanity");
    runtime_assert(map._data.size(), 4, "datasize");
    runtime_assert(map.size(), 8, "size");

    LOG_WARN("Test {} - Range erase over gap", ++num);
    map.erase(Number::N_2, Number::P_2);
    runtime_assert(vec2str(map._data), "[-4, -2, 3, 5]", "data");
    runtime_assert(!(map._data.size()&1), true, "sanity");
    runtime_assert(map._data.size(), 4, "datasize");
    runtime_assert(map.size(), 4, "size");

    LOG_WARN("Test {} - Single insert into gap", ++num);
    map.insert(Number::ZERO);
    runtime_assert(vec2str(map._data), "[-4, -2, 0, 1, 3, 5]", "data");
    runtime_assert(!(map._data.size()&1), true, "sanity");
    runtime_assert(map._data.size(), 6, "datasize");
    runtime_assert(map.size(), 5, "size");

    LOG_WARN("Test {} - Range insert onto gap", ++num);
    map.insert(Number::N_2, Number::P_2);
    runtime_assert(vec2str(map._data), "[-4, 5]", "data");
    runtime_assert(!(map._data.size()&1), true, "sanity");
    runtime_assert(map._data.size(), 2, "datasize");
    runtime_assert(map.size(), 9, "size");

    // LOG_WARN("Test {} - Range erase across upper", ++num);
    // LOG_WARN("Test {} - Range erase across lower", ++num);
    // LOG_WARN("Test {} - Single erase at upper", ++num);
    // LOG_WARN("Test {} - Single erase at lower", ++num);
    // LOG_WARN("Test {} - Single insert onto upper", ++num);
    // LOG_WARN("Test {} - Single insert onto lower", ++num);


    LOG_INFO("=== All tests for unlock_map passed! ===\n\n");
    return 0;
}
