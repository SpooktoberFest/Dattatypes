
#ifndef __DATTATYPES_BASE__HPP
#define __DATTATYPES_BASE__HPP

#include <type_traits>
#include <limits>
#include <cmath>
#include <stdexcept>


namespace Dattatypes {

// Primary template for checking if a type is an instantiation of a template
template <typename, template <typename> class>
struct is_templated : std::false_type {};
// Specialization: True if the type is an instantiation of the given template
template <typename T, template <typename> class Template>
struct is_templated<Template<T>, Template> : std::true_type {};

// The Template Class
template<typename Base, typename = typename std::enable_if_t<std::is_integral_v<Base>>>
struct Unit {
private:
    using Same = Unit<Base>;
    inline void type_assertion() {
        static_assert (
            std::is_arithmetic_v<T> || is_templated<T, Unit>::value,
            "Invalid type: Arithmetic operation is only allowed with other Units or arithmetic types."
        );
    }
public:
    constexpr Unit() : _num(static_cast<Base>(0)) {
        static_assert(std::is_integral_v<Base>, "Invalid type: Units can only be instansiated with integral types.");
    };
    constexpr Unit(const long double& val) : _num(static_cast<Base>(val * _den)) {
        static_assert(std::is_integral_v<Base>, "Invalid type: Units can only be instansiated with integral types.");
    };
    virtual ~Unit() = default;

    // Assignment Operator
    template <typename T> constexpr inline
    Same& operator=(const T& val) {
        type_assertion();
        if constexpr      (std::is_same_v<T, Unit<Base>>)   { _num = vel._num; } 
        else if constexpr (is_templated<T, Unit>::value)    { _num = static_cast<Base>(val._num / val._den * _den); } 
        else if constexpr (std::is_arithmetic_v<T>)         { _num = static_cast<Base>(val * _den); }
        return this;
    }

    // Conversion Operator
    template <typename T> constexpr explicit inline
    operator T() const {
        if constexpr      (std::is_same_v<T, Unit<Base>>)   { return static_cast<T>(_num) / _den; }  // TODO
        else if constexpr (is_templated<T, Unit>::value)    { return static_cast<T>(_num) / _den; }  // TODO
        else if constexpr (std::is_integral_v<T>)           { return static_cast<T>(_num) / _den; }  // TODO
        else if constexpr (std::is_arithmetic_v<T>)         { return static_cast<T>(_num) / _den; }  // TODO
    }

    // Comparison Operators
    template <typename T, typename L> constexpr
    bool compare(const T& val, L lambda) const {
        type_assertion();
        if constexpr (std::is_same_v<T, Unit<Base>>)        { return lambda(val._num); }
        else if constexpr (is_templated<T, Unit>::value)    { return lambda(val._num * (val._num / _den) ); }
        else if constexpr (std::is_arithmetic_v<T>)         { return lambda(val*_den / _den); }
    }
    template <typename T> constexpr bool operator==(const T& val) const {
        return compare(val, [&](const auto & rhs_num)->bool {  return _num == rhs_num; });
    }
    template <typename T> constexpr bool operator<(const T& val) const {
        return compare(val, [&](const auto & rhs_num)->bool { return _num < rhs_num; });
    }
    template <typename T> constexpr bool operator>(const T& val) const {
        return compare(val, [&](const auto & rhs_num)->bool { return _num > rhs_num; });
    }
    template <typename T> constexpr bool operator<=(const T& val) const {
        return compare(val, [&](const auto & rhs_num)->bool { return _num <= rhs_num; });
    }
    template <typename T> constexpr bool operator>=(const T& val) const {
        return compare(val, [&](const auto & rhs_num)->bool { return _num >= rhs_num; });
    }
    // (...)

    // Arithmetic Operators
    template <typename T, typename L> constexpr
    Same arithmetic(const T& val, L lambda) const {
        type_assertion(); Same res;
        if constexpr (std::is_same_v<T, Unit<Base>>)        { res._num = lambda(val._num, 1); }
        else if constexpr (is_templated<T, Unit>::value)    { res._num = lambda(val._num, val._den); }
        else if constexpr (std::is_arithmetic_v<T>)         { res._num = lambda(val, 1); }
        return res;
    }
    template <typename T> constexpr Base operator+(const T& val) const {
        return arithmetic(val, [&](const auto & rhs_num, const auto rhs_den)->Base
            { return _num + (rhs_num * (rhs_den / _den)); });
    }
    template <typename T> constexpr Base operator-(const T& val) const {
        return arithmetic(val, [&](const auto & rhs_num, const auto rhs_den)->Base
            { return _num - (rhs_num * (rhs_den / _den)); });
    }
    template <typename T> constexpr Base operator*(const T& val) const {
        return arithmetic(val, [&](const auto & rhs_num, const auto rhs_den)->Base
            { return _num * rhs_num * (rhs_den / _den); });
    }
    template <typename T> constexpr Base operator/(const T& val) const {
        return arithmetic(val, [&](const auto & rhs_num, const auto rhs_den)->Base
            { return _num * _den / (rhs_num * rhs_den); });
    }
    // (...)

    // Compound Assignment Operators
    template <typename T, typename L> constexpr
    Same& compound_assignment(const T& val, L lambda) const {
        type_assertion();
        if constexpr (std::is_same_v<T, Unit<Base>>)        { lambda(val._num, 1); }
        else if constexpr (is_templated<T, Unit>::value)    { lambda(val._num, val._den); }
        else if constexpr (std::is_arithmetic_v<T>)         { lambda(val, 1); }
        return this;
    }
    template <typename T> constexpr void operator+=(const T& val) const {
        return compound_assignment(val, [&](const auto & rhs_num, const auto rhs_den)->void
            { _num += (rhs_num * (rhs_den / _den)); });
    }
    template <typename T> constexpr void operator-=(const T& val) const {
        return compound_assignment(val, [&](const auto & rhs_num, const auto rhs_den)->void
            { _num -= (rhs_num * (rhs_den / _den)); });
    }
    template <typename T> constexpr void operator*=(const T& val) const {
        return compound_assignment(val, [&](const auto & rhs_num, const auto rhs_den)->void
            { _num *= rhs_num * (rhs_den / _den); });
    }
    template <typename T> constexpr void operator/=(const T& val) const {
        return compound_assignment(val, [&](const auto & rhs_num, const auto rhs_den)->void
            { _num *= _den / (rhs_num * rhs_den); });
    }
    // (...)

    // Member Variables
    Base _num;
    static constexpr Base _den = std::numeric_limits<Base>::max();
};

// Literals
Unit<__UINT64_TYPE__> operator"" _UU64(long double val) { return Unit<__UINT64_TYPE__>(val); }
Unit<__UINT32_TYPE__> operator"" _UU32(long double val) { return Unit<__UINT32_TYPE__>(val); }
Unit<__UINT16_TYPE__> operator"" _UU16(long double val) { return Unit<__UINT16_TYPE__>(val); }
Unit<__UINT8_TYPE__>  operator"" _UU08(long double val)  { return Unit<__UINT8_TYPE__>(val); }
Unit<__INT64_TYPE__>  operator"" _SU64(long double val) { return Unit<__INT64_TYPE__>(val); }
Unit<__INT32_TYPE__>  operator"" _SU32(long double val) { return Unit<__INT32_TYPE__>(val); }
Unit<__INT16_TYPE__>  operator"" _SU16(long double val) { return Unit<__INT16_TYPE__>(val); }
Unit<__INT8_TYPE__>   operator"" _SU08(long double val)  { return Unit<__INT8_TYPE__>(val); }

} // namespace Dattatypes

#endif
