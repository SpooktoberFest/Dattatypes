#include "my_cpp_library/my_library.hpp"
#include <cassert>
#include <iostream>

int main()
{
    assert(my_cpp_library::add(2, 3) == 5);
    std::cout << "All tests passed!\n";
    return 0;
}