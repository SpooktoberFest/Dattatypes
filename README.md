


# Dattatypes

## Preamble

A data type i have wanted for several projects for a while.
Feel free to use or modify.

*- Venlig hilsen Lukas Kronholm.*


## Dependencies
None.

## Contents

Basically a Unit is a fraction between 0 and 1 (both inclusive), but unlike floating point numbers, it utilizes its full bit range.
There is also a signed variant, that goes from -1 to 1 (excluding -1).
The idea is for this datatype to represent probabilities, angles or composents of unit complex numbers and unit quaterneons.
A unit is initialized with the aritmetic datatype it should inherit byte size and signedness from, like so:
```C++
    // (1 / 18,446,744,073,709,551,615) precision unit number.
    Unit<unsigned long long> very_presice_unit_number;

    // This would encode 256 unique, equidistant angles.
    Unit<char> angle_by_pi;
```





