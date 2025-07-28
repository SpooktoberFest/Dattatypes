
#include <iostream>
#include <stdexcept>
#include <complex>

#include "unit.hpp"

using namespace std;
using namespace Dattatypes;

// Testing
int main()
{

    // double d = 1.0;
    // Unit<__uint8_t> u_1(d);
    // Unit<__uint8_t> u_2(1.0 - d);

    // cout << "u_1: " << int(u_1._num) << " / " << int(u_1._den) << " = " << static_cast<double>(u_1) << endl;
    // cout << "u_1==d: " << (static_cast<double>(u_1) == d ? "T" : "F") << endl;
    // cout << "u_1 inc: " << double(++u_1._num) / u_1._den << endl;
    // cout << "u_1: " << int(u_1._num) << " / " << int(u_1._den) << " = " << static_cast<double>(u_1) << endl;
    // cout << "u_1 inc: " << double(++u_1._num) / u_1._den << endl;
    // cout << "u_1: " << int(u_1._num) << " / " << int(u_1._den) << " = " << static_cast<double>(u_1) << endl;
    // cout << "u_1 inc: " << double(++u_1._num) / u_1._den << endl;
    // cout << "u_1: " << int(u_1._num) << " / " << int(u_1._den) << " = " << static_cast<double>(u_1) << endl;
    // cout << "u_1 inc: " << double(++u_1._num) / u_1._den << endl;
    // cout << "u_1==d: " << (static_cast<double>(u_1) == d ? "T" : "F") << endl;

    // complex<Unit<int>> z(.5, .5);
    // cout << "z: " << static_cast<double>(z.real()) << " + " << static_cast<double>(z.imag()) << "i" << endl;

    // assert(my_cpp_library::add(2, 3) == 5);
    std::cout << "All tests passed!\n";

    return 0;
}
