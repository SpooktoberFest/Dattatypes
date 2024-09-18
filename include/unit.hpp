
#ifndef __DATTATYPES_UNIT__HPP
#define __DATTATYPES_UNIT__HPP

#include <type_traits>
#include <limits>
#include <cmath>
#include <stdexcept>
//
#include "dattatypes.hpp"


namespace Dattatypes {

template<typename Base, typename = std::enable_if_t<std::is_integral_v<Base>>>
class Unit {
private:
    using Same = Unit<Base>;
    template <typename T> constexpr
    inline void type_assertion(const T& value) {
        static_assert (
            std::is_arithmetic_v<T> || is_templated<T, Unit>::value,
            "Invalid type: This type of operation is only allowed with other Units or arithmetic types.");
    }


public:
    constexpr Unit() : _num(Base(0)) {
        static_assert(
            std::is_integral_v<Base>,
            "Invalid type: Units can only be instansiated with integral types.");
    };
    template <typename T> constexpr
    Unit(const T& other) : _num(Base(other * _den)) {
        static_assert(
            std::is_integral_v<Base>,
            "Invalid type: Units can only be instansiated with integral types.");
        type_assertion(other);
    };
    virtual ~Unit() = default;
    
    // Member Variables
    Base _num;
    static constexpr Base _den = std::numeric_limits<Base>::max()
        - (std::is_signed_v<Base> ? 1 : 0);

    // Misc Public Mehods
    bool constexpr inline is_max() { return _num == _den; }
    bool constexpr inline is_min() { return _num == Base(_den)+1; }


    // Assignment Operator
    template <typename T> constexpr inline
    Same& operator=(const T& other) {
        type_assertion(other);
        if constexpr      (std::is_same_v<T, Unit<Base>>)   { _num = other._num; } 
        else if constexpr (is_templated<T, Unit>::value)    { _num = Base(other._num / other._den * _den); } 
        else if constexpr (std::is_arithmetic_v<T>)         { _num = Base(other * _den); }
        return this;
    }
    // */

    // Conversion Operator
    template <typename T> constexpr explicit inline
    operator T() const {
        if constexpr      (std::is_same_v<T, bool>)         { return bool(_num); }
        // else if constexpr (is_templated<T, Unit>::value)    { return T(_num) / _den; } // Not recommended
        else if constexpr (std::is_arithmetic_v<T>)         { return T(_num) / _den; }
    }
    // */

    // Comparison Operators
    template <typename T, typename L> constexpr inline
    bool compare(const T& other, L lambda) const {
        type_assertion(other);
        if constexpr (std::is_same_v<T, Unit<Base>>)        { return lambda(other._num); }
        else if constexpr (is_templated<T, Unit>::value)    { return lambda(other._num * (other._num / _den) ); }
        else if constexpr (std::is_arithmetic_v<T>)         { return lambda(other*_den / _den); }
    }
    template <typename T> constexpr bool operator==(const T& other) const {
        return compare(other, [&](const auto & other_num)->bool {  return _num == other_num; });
    }
    template <typename T> constexpr bool operator<(const T& other) const {
        return compare(other, [&](const auto & other_num)->bool { return _num < other_num; });
    }
    template <typename T> constexpr bool operator>(const T& other) const {
        return compare(other, [&](const auto & other_num)->bool { return _num > other_num; });
    }
    template <typename T> constexpr bool operator<=(const T& other) const {
        return compare(other, [&](const auto & other_num)->bool { return _num <= other_num; });
    }
    template <typename T> constexpr bool operator>=(const T& other) const {
        return compare(other, [&](const auto & other_num)->bool { return _num >= other_num; });
    }
    // (...) */

    // Arithmetic Operators
    template <typename T, typename L> constexpr inline
    Same arithmetic(const T& other, L lambda) const {
        type_assertion(other); Same res;
        if constexpr (std::is_same_v<T, Unit<Base>>)        { res._num = lambda(other._num, 1); }
        else if constexpr (is_templated<T, Unit>::value)    { res._num = lambda(other._num, other._den); }
        else if constexpr (std::is_arithmetic_v<T>)         { res._num = lambda(other, 1); }
        return res;
    }
    template <typename T> constexpr Base operator+(const T& other) const {
        return arithmetic(other, [&](const auto & other_num, const auto other_den)->Base
            { return _num + (other_num * (other_den / _den)); });
    }
    template <typename T> constexpr Base operator-(const T& other) const {
        return arithmetic(other, [&](const auto & other_num, const auto other_den)->Base
            { return _num - (other_num * (other_den / _den)); });
    }
    template <typename T> constexpr Base operator*(const T& other) const {
        return arithmetic(other, [&](const auto & other_num, const auto other_den)->Base
            { return _num * other_num * (other_den / _den); });
    }
    template <typename T> constexpr Base operator/(const T& other) const {
        return arithmetic(other, [&](const auto & other_num, const auto other_den)->Base
            { return _num * _den / (other_num * other_den); });
    }
    // (...) */

    // Compound Assignment Operators
    template <typename T, typename L> constexpr inline
    Same& compound_assignment(const T& other, L lambda) const {
        type_assertion(other);
        if constexpr (std::is_same_v<T, Unit<Base>>)        { lambda(other._num, 1); }
        else if constexpr (is_templated<T, Unit>::value)    { lambda(other._num, other._den); }
        else if constexpr (std::is_arithmetic_v<T>)         { lambda(other, 1); }
        return this;
    }
    template <typename T> constexpr void operator+=(const T& other) const {
        return compound_assignment(other, [&](const auto & other_num, const auto other_den)->void
            { _num += (other_num * (other_den / _den)); });
    }
    template <typename T> constexpr void operator-=(const T& other) const {
        return compound_assignment(other, [&](const auto & other_num, const auto other_den)->void
            { _num -= (other_num * (other_den / _den)); });
    }
    template <typename T> constexpr void operator*=(const T& other) const {
        return compound_assignment(other, [&](const auto & other_num, const auto other_den)->void
            { _num *= other_num * (other_den / _den); });
    }
    template <typename T> constexpr void operator/=(const T& other) const {
        return compound_assignment(other, [&](const auto & other_num, const auto other_den)->void
            { _num *= _den / (other_num * other_den); });
    }
    // (...) */


};


/*/ Literals
Complex<__UINT64_TYPE__> operator"" _UU64(long double val) { return Complex<__UINT64_TYPE__>(val); }
Complex<__UINT32_TYPE__> operator"" _UU32(long double val) { return Complex<__UINT32_TYPE__>(val); }
Complex<__UINT16_TYPE__> operator"" _UU16(long double val) { return Complex<__UINT16_TYPE__>(val); }
Complex<__UINT8_TYPE__>  operator"" _UU08(long double val)  { return Complex<__UINT8_TYPE__>(val); }
Complex<__INT64_TYPE__>  operator"" _SU64(long double val) { return Complex<__INT64_TYPE__>(val); }
Complex<__INT32_TYPE__>  operator"" _SU32(long double val) { return Complex<__INT32_TYPE__>(val); }
Complex<__INT16_TYPE__>  operator"" _SU16(long double val) { return Complex<__INT16_TYPE__>(val); }
Complex<__INT8_TYPE__>   operator"" _SU08(long double val)  { return Complex<__INT8_TYPE__>(val); }
//*/

} // namespace Dattatypes


#endif // __DATTATYPES_UNIT__HPP
