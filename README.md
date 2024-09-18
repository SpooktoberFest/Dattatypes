


# Dattatypes

## Preamble

A data type i have wanted for several projects for a while.
Feel free to use or modify.

*- Venlig hilsen Lukas Kronholm.*


## Dependencies
None.

## Contents

# Unit

Basically a Unit is a fraction between 0 and 1 (both inclusive), but unlike floating point numbers, it utilizes its full bit range.
There is also a signed variant, that goes from -1 to 1 (excluding -1).
The idea is for this datatype to represent probabilities, angles or composents of unit complex numbers and unit quaterneons.
A unit is initialized with the aritmetic datatype it should inherit byte size and signedness from, like so:
```C++
    using namespace Dattatypes;

    // (1 / 2^2^8) precision unit number.
    Unit<unsigned long long> very_presice_unit_number(0.00043453);

    // This would encode 256 unique, equidistant angles.
    Unit<char> angle_by_pi = 0.5_SU08;

    // These may be of particular interest.
    Complex<Unit<int>> u_z;
    Quaterneon<Unit<int>> u_q;

```

When defining (and ideally also when using) operator overloads for Units, lhs should be assumed/made to have greater resolution than rhs. This is not a strict requirement.

# Unit


# Unit



