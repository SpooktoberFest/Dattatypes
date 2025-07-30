
#include <iostream>
#include <stdexcept>

#include "debug.hpp"
#include "prec.hpp"

using namespace std;
using namespace Dattatypes;

// Testing
int main()
{
    // 32 bit Prec with 8 fractional bits (1/255 point precision)
    typedef Dattatypes::Prec<int, 8> prec32_8;

    constexpr prec32_8 a(2);     // 2.0
    constexpr prec32_8 b(0.5);   // 0.5
    constexpr prec32_8 c(3);     // 3.0

    // Addition
    static_assert((a + b) == prec32_8(2.5), "Addition failed step 1");
    static_assert(double(b + c) == (3.5), "Addition failed step 2");
    static_assert((a + 4) == prec32_8(6), "Addition failed step 3");

    // Subtraction
    static_assert((a - b) == prec32_8(1.5), "Subtraction failed step 1");
    static_assert(double(b - c) == (-2.5), "Subtraction failed step 2");
    static_assert((a - 2) == prec32_8(0.0), "Subtraction failed step 3");

    // Multiplication
    static_assert((a * b) == prec32_8(1.0), "Multiplication failed step 1");
    static_assert(double(b * c) == (1.5), "Multiplication failed step 2");
    static_assert((a * 4) == prec32_8(8.0), "Multiplication failed step 3");

    // Division
    static_assert((a / b) == prec32_8(4.0), "Division failed step 1");
    static_assert(double(c / b) == (6), "Division failed step 2");
    static_assert((a / 10) == prec32_8(0.2), "Division failed step 3");





    return 0;
}
