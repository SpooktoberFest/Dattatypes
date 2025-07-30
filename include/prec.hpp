#pragma once

#include <type_traits>

namespace Dattatypes {

    /**
     * Additional Precision Integers : Template class Prec
     *
     */
    template <typename T, int order>
    class Prec {
        static_assert(std::is_integral_v<T>, "T must be an integer type");
        static_assert(order < sizeof(T)*8, "Order must be smaller that the bitsize of T");

        using ThisType = Prec<T, order>;
        static constexpr T _order = order;
        static constexpr T _2order = order<<1;

        constexpr Prec(T raw_data, bool is_raw) : _data(raw_data) {}

    public:
        T _data;

        constexpr Prec() = default;
        constexpr ~Prec() = default;
        constexpr Prec(int value) : _data(static_cast<T>(value << _order)) {}
        constexpr Prec(double value) : _data(static_cast<T>(value * (1 << _order))) {}

        // Comparison Operators:        ( ==, !=, >, <, >=, <= )
        constexpr bool operator==(ThisType other) const { return _data == other._data; }
        constexpr bool operator!=(ThisType other) const { return _data != other._data; }
        constexpr bool operator<(ThisType other) const { return _data < other._data; }
        constexpr bool operator>(ThisType other) const { return _data > other._data; }
        constexpr bool operator<=(ThisType other) const { return _data <= other._data; }
        constexpr bool operator>=(ThisType other) const { return _data >= other._data; }

        // Assignment Operators:        ( = )
        constexpr ThisType& operator=(ThisType other) { _data = other._data; return *this; }
        // - Aritmetic                  ( +=, -=, *=, /=, %= )
        constexpr ThisType& operator+=(const T other) { _data += (other << _order); return *this; }
        constexpr ThisType& operator-=(const T other) { _data -= (other << _order); return *this; }
        constexpr ThisType& operator*=(const T other) { _data *= other; return *this; }
        constexpr ThisType& operator/=(const T other) { _data /= other; return *this; }
        constexpr ThisType& operator+=(const double other) { _data += other; return *this; }
        constexpr ThisType& operator-=(const double other) { _data -= other; return *this; }
        constexpr ThisType& operator*=(const double other) { _data *= other; return *this; }
        constexpr ThisType& operator/=(const double other) { _data /= other; return *this; }
        constexpr ThisType& operator+=(ThisType other) { _data += other._data; return *this; }
        constexpr ThisType& operator-=(ThisType other) { _data -= other._data; return *this; }
        constexpr ThisType& operator*=(ThisType other) { (_data *= other._data) <<= (_2order); return *this; }
        constexpr ThisType& operator/=(ThisType other) { (_data /= other._data) >>= (_2order); return *this; }
        // - Bitwise                    ( >>=, <<=, &=, ^=, |= )
        constexpr ThisType& operator>>=(const int other) { _data >>= other; return *this; }
        constexpr ThisType& operator<<=(const int other) { _data <<= other; return *this; }
        constexpr ThisType& operator&=(const T other) { _data &= other; return *this; }
        constexpr ThisType& operator^=(const T other) { _data ^= other; return *this; }
        constexpr ThisType& operator|=(const T other) { _data |= other; return *this; }
        constexpr ThisType& operator&=(const ThisType other) { _data &= other._data; return *this; }
        constexpr ThisType& operator^=(const ThisType other) { _data ^= other._data; return *this; }
        constexpr ThisType& operator|=(const ThisType other) { _data |= other._data; return *this; }

        // Aritmetic Operators:         ( +, -, *, /, % )
        constexpr ThisType operator+(const T other) const { return ThisType(_data + (other << _order), true); }
        constexpr ThisType operator-(const T other) const { return ThisType(_data - (other << _order), true); }
        constexpr ThisType operator*(const T other) const { return ThisType(_data * other, true); }
        constexpr ThisType operator/(const T other) const { return ThisType(_data / other, true); }
        constexpr ThisType operator+(const double other) const { return ThisType(_data + other, true); }
        constexpr ThisType operator-(const double other) const { return ThisType(_data - other, true); }
        constexpr ThisType operator*(const double other) const { return ThisType(_data * other, true); }
        constexpr ThisType operator/(const double other) const { return ThisType(_data / other, true); }
        constexpr ThisType operator+(const ThisType other) const { return ThisType(_data + other._data, true); }
        constexpr ThisType operator-(const ThisType other) const { return ThisType(_data - other._data, true); }
        constexpr ThisType operator*(const ThisType other) const { return ThisType((_data * other._data) >> _order, true); }
        constexpr ThisType operator/(const ThisType other) const { return ThisType((_data << _order) / other._data, true); }

        // Unary Operators:
        constexpr bool operator!() const { return bool(!_data); }
        constexpr ThisType& operator++() { _data += (1<<_order); return *this; }
        constexpr ThisType& operator--() { _data -= (1<<_order); return *this; }
        constexpr ThisType operator-() const { return ThisType(-_data, true); }
        constexpr ThisType operator~() const { return ThisType(~_data, true); }

        // Bitwise  Operators:          ( >>=, <<=, &=, ^=, |= )
        constexpr ThisType operator>>(const int other) const { return ThisType(_data >>= other, true); }
        constexpr ThisType operator<<(const int other) const { return ThisType(_data <<= other, true); }
        constexpr ThisType operator&(T other) const { return ThisType(_data &= other, true); }
        constexpr ThisType operator^(T other) const { return ThisType(_data ^= other, true); }
        constexpr ThisType operator|(T other) const { return ThisType(_data |= other, true); }
        constexpr ThisType operator&(ThisType other) const { return ThisType(_data &= other._data, true); }
        constexpr ThisType operator^(ThisType other) const { return ThisType(_data ^= other._data, true); }
        constexpr ThisType operator|(ThisType other) const { return ThisType(_data |= other._data, true); }

        // Conversion Operators:        (T, bool)
        constexpr operator T() const { return _data >> _order; }
        constexpr operator bool() const { return _data != 0; }
        constexpr operator double() const { return static_cast<double>(_data) / (1 << _order); }

        // Returns sub-one component
        constexpr ThisType remainder() const { return ThisType(_data & ~((1<<_order)-1), true); };
    };




}; // namespace Dattatypes
