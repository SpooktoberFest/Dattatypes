
#include <iostream>

#include "debug.hpp"
#include "prec_utils.hpp"

static constexpr auto src = "Prec:TEST";
using namespace std;
using namespace dattatypes;


// Testing
int main() {
    LOG_INFO("=== Beginning Tests for Prec ===");

    constexpr prec32 a(2);
    constexpr prec32 b(0.5);
    constexpr prec32 c = 3.0;

    // Addition
    static_assert((a + b) == prec32(2.5), "Addition failed step 1");
    static_assert(double(b + c) == (3.5), "Addition failed step 2");
    static_assert((a + 4) == prec32(6.0), "Addition failed step 3");
    static_assert((a + (-2)) == prec32(0.0), "Addition with negative integer failed");
    static_assert((a + (-2.0)) == prec32(0.0), "Addition with negative double failed");
    static_assert((b + 0) == prec32(0.5), "Addition with zero failed");

    // Subtraction
    static_assert((a - b) == prec32(1.5), "Subtraction failed step 1");
    static_assert(double(b - c) == (-2.5), "Subtraction failed step 2");
    static_assert((a - 2) == prec32(0.0), "Subtraction failed step 3");
    static_assert((c - (-1)) == prec32(4.0), "Subtraction with negative integer failed");
    static_assert((c - (-1.0)) == prec32(4.0), "Subtraction with negative double failed");

    // Multiplication
    static_assert((a * b) == prec32(1.0), "Multiplication failed step 1");
    static_assert(double(b * c) == (1.5), "Multiplication failed step 2");
    static_assert((a * 4) == prec32(8.0), "Multiplication failed step 3");
    static_assert((a * 0) == prec32(0.0), "Multiplication by zero failed");
    static_assert((b * (-2)) == prec32(-1.0), "Multiplication by negative integer failed");
    static_assert((b * (-2.0)) == prec32(-1.0), "Multiplication by negative double failed");

    // Division
    static_assert((a / b) == prec32(4.0), "Division failed step 1");
    static_assert(double(c / b) == (6.0), "Division failed step 2");
    static_assert((a / 10) == prec32(0.2), "Division failed step 3");
    static_assert((c / (-3)) == prec32(-1.0), "Division by negative integer failed");
    static_assert((c / (-3.0)) == prec32(-1.0), "Division by negative double failed");

    // Unary Minus
    static_assert((-a) == prec32(-2.0), "Unary minus failed");
    static_assert((-b) == prec32(-0.5), "Unary minus failed on fractional");

    // Conversion Tests
    static_assert(int(prec32(2.9)) == 2, "Integer conversion failed");
    static_assert(bool(prec32(0.0)) == false, "Boolean conversion failed for zero");
    static_assert(bool(prec32(0.1)) == true, "Boolean conversion failed for non-zero");

    // Chained Operations
    static_assert(((a + b) * c) == prec32(7.5), "Chained addition and multiplication failed");
    static_assert(((a - b) / b) == prec32(3.0), "Chained subtraction and division failed");

    // Equality with Different Representations
    static_assert((prec32(2.5) == (a + b)), "Equality after addition failed");
    static_assert((prec32(0.5) == b), "Direct equality failed");

    // Commutativity
    static_assert((a + b) == (b + a), "Addition commutativity failed");
    static_assert((a * b) == (b * a), "Multiplication commutativity failed");

    // Approximation
    static_assert(prec8(5.2).approx(5), "Approximation failed step 1");
    static_assert(prec8(5).fapprox(5.2), "Approximation failed step 2");
    static_assert(!(prec8(5.3).approx(5)), "Approximation failed step 3");
    static_assert(!(prec8(5).fapprox(5.3)), "Approximation failed step 4");

    // Square Root
    static_assert(sqrt(prec32(81)) == prec32(9), "Square Root failed step 1");
    constexpr int x = 834;
    static_assert(sqrt(prec32(x*x)) == prec32(x), "Square Root failed step 2");

    LOG_INFO("=== All tests for Prec passed! ===\n\n");
    return 0;
}
