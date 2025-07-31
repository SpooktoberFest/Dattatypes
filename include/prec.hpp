#pragma once

#include <type_traits>
#include <cmath>

namespace Dattatypes {

    /**
     * Fixed Precision Integers : Template class Prec
     *
     * The `Prec` template implements a fixed-point numeric type using integer arithmetic.
     * It represents fractional numbers with a configurable precision level by scaling integer
     * values via bit shifting.
     *
     * Template Parameters:
     * - T     : The underlying integer type (e.g., int8_t, int16_t, int32_t, uint64_t).
     *           Must be an integral type.
     * - order : The number of fractional bits used for precision scaling.
     *           - A positive `order` defines fractional precision, where the fractional unit is (1 / 2^order).
     *           - A negative `order` implies left-shifting the input value, effectively scaling integers up.
     *             This can be used for unit multipliers or handling larger ranges at the cost of fractional precision.
     *
     * Concept:
     * - Internally stores a scaled integer (`_data`) representing the fixed-point number.
     *   - For `order > 0`, values are scaled by (1 << order) to simulate fractional precision.
     *   - For `order < 0`, values are scaled down by (1 >> |order|), effectively amplifying the integer range.
     * - Arithmetic and bitwise operators are overloaded to handle the appropriate scaling logic,
     *   maintaining consistency in fixed-point operations.
     * - Conversion operators are provided to extract the integer component or cast to floating-point values.
     *
     * Usage Constraints & Considerations:
     * - Not all operators or operations make semantic sense for every usage of `Prec`.
     *   For example:
     *   - Increment (++) or adding literal integers may not be meaningful for normalized types like unit numbers
     *     (e.g., numbers constrained within [-1.0;1.0) or [0.0;1.0) ranges).
     *   - Bitwise operations are valid at the raw integer level but can disrupt the numeric meaning in derived types.
     *   - Derived types like probabilities, angles, and unit vectors will impose logical constraints on what arithmetic
     *     operations are valid or meaningful.
     * - When designing specific aliases (e.g., `unit32`, `angle16`), users should be mindful of which operators
     *   are logically applicable to prevent misuse.
     *
     * Advantages:
     * - Fixed-point arithmetic is faster and more deterministic than floating-point on architectures without FPUs.
     * - They utilize the full bit range of the underlying type to represent fractional or scaled integer values efficiently.
     * - Useful in embedded systems, graphics shaders, signal processing, and deterministic simulations where
     *   floating-point inaccuracies are undesirable.
     *
     * Limitations:
     * - Precision loss can occur during divisions or repeated multiplications without appropriate scaling.
     * - Overflow risks depend on the size of T and the value of `order`. Higher `order` reduces effective numeric range.
     * - Negative `order` shifts can increase numeric range but sacrifice precision at low values.
     * - Logical misuse of operators (e.g., increment on unit types) is not type-enforced by the class and requires
     *   usage discipline.
     */
    template <typename T, int order>
    class Prec {
        static_assert(std::is_integral_v<T>, "T must be an integer type");
        using ThisType = Prec<T, order>;
    public:
        using value_type = T;
        static constexpr T _n = -order; // Amount by which incoming ints should be left-shifted.
    private:
        // Factor for converting incoming floats or negative numbers.
        static constexpr float _f = (_n >= 0) ? float(T(1) << _n) : 1.0f / float(T(1) << -_n);
        static constexpr float _if = 1/_f;

        // Scale from integer space to data space
        constexpr int scale(const int value) const {
            if constexpr (_n >= 0)
                return (value >= 0) ? value << _n : value * _f;
            else
                return (value >= 0) ? value >> (-_n) : value * _f;
        }
        // Scale from data space to integer space
        constexpr long iscale(T value) const {
            if constexpr (_n >= 0)
                return (value >= 0) ? value >> _n : value * _if;
            else
                return (value >= 0) ? value << (-_n) : value * _if;
        }
        // Scale from float space to data space
        constexpr int fscale(const double value) const { return int(value * _f); }


        constexpr Prec(T raw_data, bool is_raw) : _data(raw_data) {}

    public:
        T _data;

        constexpr Prec() = default;
        constexpr ~Prec() = default;
        constexpr Prec(const int value) : _data(T(scale(value))) {}
        constexpr Prec(const double value) : _data(T(fscale(value))) {}
        constexpr Prec(const ThisType& copy_from) : _data(copy_from._data) {}
        constexpr Prec(ThisType&& move_from) : _data(move_from._data) { move_from._data=0; }

        // Comparison Operators: ( ==, !=, >, <, >=, <= )
        constexpr bool operator==(const ThisType other) const { return _data == other._data; }
        constexpr bool operator!=(const ThisType other) const { return _data != other._data; }
        constexpr bool operator<(const ThisType other) const { return _data < other._data; }
        constexpr bool operator>(const ThisType other) const { return _data > other._data; }
        constexpr bool operator<=(const ThisType other) const { return _data <= other._data; }
        constexpr bool operator>=(const ThisType other) const { return _data >= other._data; }

        // Direct Assignment Operators: ( = )
        constexpr ThisType& operator=(const int value) { _data = scale(value); return *this; }
        constexpr ThisType& operator=(const double value) { _data = fscale(value); return *this; }
        constexpr ThisType& operator=(const ThisType& other) { _data = other._data; return *this; }
        constexpr ThisType& operator=(ThisType&& other) { _data = other._data; other._data=0; return *this; }

        // Aritmetic Assignment Operators: ( +=, -=, *=, /= )
        constexpr ThisType& operator+=(const int value) { _data += scale(value); return *this; }
        constexpr ThisType& operator-=(const int value) { _data -= scale(value); return *this; }
        constexpr ThisType& operator*=(const int value) { _data *= value; return *this; }
        constexpr ThisType& operator/=(const int value) { _data /= value; return *this; }
        constexpr ThisType& operator+=(const double value) { _data += value * _f; return *this; }
        constexpr ThisType& operator-=(const double value) { _data -= value * _f; return *this; }
        constexpr ThisType& operator*=(const double value) { _data *= value; return *this; }
        constexpr ThisType& operator/=(const double value) { _data /= value; return *this; }
        constexpr ThisType& operator+=(const ThisType other) { _data += other._data; return *this; }
        constexpr ThisType& operator-=(const ThisType other) { _data -= other._data; return *this; }
        constexpr ThisType& operator*=(const ThisType other) { (_data *= other._data) >>= (_n); return *this; }
        constexpr ThisType& operator/=(const ThisType other) { ((_data <<= _n) /= other._data); return *this; }

        // Bitwise Assignment Operators: ( >>=, <<=, &=, ^=, |= )
        constexpr ThisType& operator>>=(const unsigned value) { _data >>= value; return *this; }
        constexpr ThisType& operator<<=(const unsigned value) { _data <<= value; return *this; }
        constexpr ThisType& operator&=(const int value) { _data &= value; return *this; }
        constexpr ThisType& operator^=(const int value) { _data ^= value; return *this; }
        constexpr ThisType& operator|=(const int value) { _data |= value; return *this; }
        constexpr ThisType& operator&=(const ThisType other) { _data &= other._data; return *this; }
        constexpr ThisType& operator^=(const ThisType other) { _data ^= other._data; return *this; }
        constexpr ThisType& operator|=(const ThisType other) { _data |= other._data; return *this; }

        // Aritmetic Operators:         ( +, -, *, / )
        constexpr ThisType operator+(const int value) const { return ThisType(_data + scale(value), true); }
        constexpr ThisType operator-(const int value) const { return ThisType(_data - scale(value), true); }
        constexpr ThisType operator*(const int value) const { return ThisType(_data * value, true); }
        constexpr ThisType operator/(const int value) const { return ThisType(_data / value, true); }
        constexpr ThisType operator+(const double value) const { return ThisType(_data + fscale(value), true); }
        constexpr ThisType operator-(const double value) const { return ThisType(_data - fscale(value), true); }
        constexpr ThisType operator*(const double value) const { return ThisType(_data * value, true); }
        constexpr ThisType operator/(const double value) const { return ThisType(_data / value, true); }
        constexpr ThisType operator+(const ThisType other) const { return ThisType(_data + other._data, true); }
        constexpr ThisType operator-(const ThisType other) const { return ThisType(_data - other._data, true); }
        constexpr ThisType operator*(const ThisType other) const { return ThisType(iscale(_data * other._data), true); }
        constexpr ThisType operator/(const ThisType other) const { return ThisType(scale(_data) / other._data, true); }

        // Unary Operators: ( !, ++, --, -, ~ )
        constexpr bool operator!() const { return bool(!_data); }
        constexpr ThisType& operator++() { _data += scale(1); return *this; }
        constexpr ThisType& operator--() { _data -= scale(1); return *this; }
        constexpr ThisType operator-() const { return ThisType(-_data, true); }
        constexpr ThisType operator~() const { return ThisType(~_data, true); }

        // Bitwise  Operators:          ( >>=, <<=, &=, ^=, |= )
        constexpr ThisType operator>>(const unsigned value) const { return ThisType(_data >>= value, true); }
        constexpr ThisType operator<<(const unsigned value) const { return ThisType(_data <<= value, true); }
        constexpr ThisType operator&(const int value) const { return ThisType(_data &= value, true); }
        constexpr ThisType operator^(const int value) const { return ThisType(_data ^= value, true); }
        constexpr ThisType operator|(const int value) const { return ThisType(_data |= value, true); }
        constexpr ThisType operator&(const ThisType other) const { return ThisType(_data &= other._data, true); }
        constexpr ThisType operator^(const ThisType other) const { return ThisType(_data ^= other._data, true); }
        constexpr ThisType operator|(const ThisType other) const { return ThisType(_data |= other._data, true); }

        // Conversion Operators:        (int, bool, double, Prec)
        constexpr operator int() const { return iscale(_data); }
        constexpr operator bool() const { return _data != 0; }
        constexpr operator double() const { return double(_data * _if); }
        template<typename OtherPrec>
        constexpr explicit operator OtherPrec() const {
            constexpr int shift = _n - OtherPrec::_n;
            using OtherT = typename OtherPrec::value_type;
            typename std::conditional_t<(sizeof(T) > sizeof(OtherT)), T, OtherT> value = _data;

            if constexpr (shift > 0)        value = value >> shift;
            else if constexpr (shift < 0)   value = value << (-shift);

            return OtherPrec(static_cast<OtherT>(value), true);
        }

        // Clamping
        constexpr ThisType& clamp(int maxval) { if (_data > scale(maxval)) _data = scale(maxval); return *this; }
        constexpr ThisType& clamp(double maxval) { if (_data > (maxval * _f)) _data = (maxval * _f); return *this; }
        constexpr ThisType& clamp(ThisType max) { if (_data > max._data) _data = max._data; return *this; }

        // Approximate Equality (epsilon is the resolution)
        constexpr bool approx(T value) const {
            return (_data > scale(value) - 1) && (_data < scale(value) + 1);
        }
        constexpr bool fapprox(const double value) const {
            T scaled = fscale(value);
            return (_data > scaled - 1) && (_data < scaled + 1);
        }
    };

    // Prec Trait
    template <typename T>
    struct is_prec : std::false_type {};
    template <typename... Args>
    struct is_prec<Prec<Args...>> : std::true_type {};


}; // namespace Dattatypes
