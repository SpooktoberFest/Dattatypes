
#include <iostream>
#include <stdexcept>
#include <complex>

#include "unit.hpp"

using namespace std;
using namespace Dattatypes;

// Testing
int main(int argc, char* args[]) {

    double d = 1.0;
    Unit<__uint8_t> u(d);

    cout << "u: " << int(u._num) << " / " << int(u._den) << " = " << static_cast<double>(u) << endl;
    cout << "u==d: " << (static_cast<double>(u) == d ? "T":"F") << endl;
    cout << "u inc: " << double(++u._num) / u._den << endl;
    cout << "u: " << int(u._num) << " / " << int(u._den) << " = " << static_cast<double>(u) << endl;
    cout << "u inc: " << double(++u._num) / u._den << endl;
    cout << "u: " << int(u._num) << " / " << int(u._den) << " = " << static_cast<double>(u) << endl;
    cout << "u inc: " << double(++u._num) / u._den << endl;
    cout << "u: " << int(u._num) << " / " << int(u._den) << " = " << static_cast<double>(u) << endl;
    cout << "u inc: " << double(++u._num) / u._den << endl;
    cout << "u==d: " << (static_cast<double>(u) == d ? "T":"F") << endl;


    complex<Unit<int>> z(.5, .5);
    cout << "z: " << static_cast<double>(z.real()) << " + " << static_cast<double>(z.imag()) << "i" << endl;



    return 0;
}
