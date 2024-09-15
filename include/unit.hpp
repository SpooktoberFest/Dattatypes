
#ifndef __DATTATYPES_BASE__HPP
#define __DATTATYPES_BASE__HPP

#include <type_traits>
#include <limits>
#include <cmath>
#include <stdexcept>



namespace Dattatypes {

// Primary template for checking if a type is an instantiation of a template
template <typename, template <typename> class>
struct is_instantiation_of : std::false_type {};
// Specialization: True if the type is an instantiation of the given template
template <typename T, template <typename> class Template>
struct is_instantiation_of<Template<T>, Template> : std::true_type {};

// Macros
#define type_check_same if constexpr (std::is_same<T, Unit<Base>>::value)
#define type_check_unit if constexpr (is_instantiation_of<T, Unit>::value)
#define type_check_arit if constexpr (std::is_arithmetic<T>::value)
#define type_assertion static_assert (std::is_arithmetic<T>::value || is_instantiation_of<T, Unit>::value, \
    "Invalid type: Arithmetic operation is only allowed with other Units or arithmetic types.");


// Template Class
template<typename Base, typename = typename std::enable_if_t<std::is_integral_v<Base>>>
struct Unit {
private:
    using Same = Unit<Base>;
public:
    constexpr Unit() : _num(static_cast<Base>(0)) {
        static_assert(std::is_integral<Base>::value, "Invalid type: Units can only be instansiated with integral types.");
    };
    constexpr Unit(const long double& val) : _num(static_cast<Base>(val * _den)) {
        static_assert(std::is_integral<Base>::value, "Invalid type: Units can only be instansiated with integral types.");
    };
    virtual ~Unit() = default;

    // Assignment Operator
    template <typename T> constexpr inline
    Same& operator=(const T& val) {
        type_assertion
        type_check_same      { _num = vel._num; } 
        else type_check_unit { _num = static_cast<Base>(val._num / val._den * _den); } 
        else type_check_arit { _num = static_cast<Base>(val * _den); }
        return this;
    }

    // Conversion Operator
    template <typename T> constexpr explicit inline
    operator T() const {
        type_check_unit      { return static_cast<T>(_num) / _den; }  // TODO
        else type_check_arit { return static_cast<T>(_num) / _den; }        
    }

    // Comparison Operators
    template <typename T, typename L> constexpr
    bool compare(const T& val, L lambda) const {
        type_assertion
        type_check_same { return lambda(_num, val._num); }
        else type_check_unit { return lambda(_num * _den, val._num * val._den); }
        else type_check_arit { return lambda(val * _den, static_cast<T>(_num)); }
    }
    template <typename T> constexpr bool operator==(const T& val) const {
        return compare(val, [&](const auto & lhs, const auto & rhs) { return lhs == rhs; });
    }
    template <typename T> constexpr bool operator<(const T& val) const {
        return compare(val, [&](const auto & lhs, const auto & rhs) { return lhs < rhs; });
    }
    template <typename T> constexpr bool operator>(const T& val) const {
        return compare(val, [&](const auto & lhs, const auto & rhs) { return lhs > rhs; });
    }
    template <typename T> constexpr bool operator<=(const T& val) const {
        return compare(val, [&](const auto & lhs, const auto & rhs) { return lhs <= rhs; });
    }
    template <typename T> constexpr bool operator>=(const T& val) const {
        return compare(val, [&](const auto & lhs, const auto & rhs) { return lhs >= rhs; });
    }

    // Arithmetic Operators
    template <typename T, typename L> constexpr
    Same arithmetic(const T& val, L lambda) const {
        type_assertion Same res;
        type_check_same { res._num = lambda(_num, val._num); }
        else type_check_unit { res._num = lambda(_num * _den, val._num * val._den); }
        else type_check_arit { res._num = lambda(val * _den, static_cast<T>(_num)); } // TODO
        return res;
    }
    template <typename T> constexpr Base operator+(const T& val) const {
        return arithmetic(val, [&](const auto & lhs, const auto & rhs) { return lhs + rhs; });
    }
    template <typename T> constexpr Base operator-(const T& val) const {
        return arithmetic(val, [&](const auto & lhs, const auto & rhs) { return lhs - rhs; });
    }
    template <typename T> constexpr Base operator*(const T& val) const {
        return arithmetic(val, [&](const auto & lhs, const auto & rhs) { return lhs * rhs; });
    }
    template <typename T> constexpr Base operator/(const T& val) const {
        return arithmetic(val, [&](const auto & lhs, const auto & rhs) { return lhs / rhs; });
    }

    // Compound Assignment Operators
    template <typename T, typename L> constexpr
    Same& compound_assignment(const T& val, L lambda) const {
        type_assertion
        type_check_same {  lambda(_num, val._num); }
        else type_check_unit { lambda(_num * _den, val._num * val._den); }
        else type_check_arit { lambda(val * _den, static_cast<T>(_num)); }
        return this;
    }
    template <typename T> constexpr void operator+=(const T& val) const {
        return compound_assignment(val, [&](auto & lhs, const auto & rhs) { lhs += rhs; });
    }
    template <typename T> constexpr void operator-=(const T& val) const {
        return compound_assignment(val, [&](auto & lhs, const auto & rhs) { lhs -= rhs; });
    }
    template <typename T> constexpr void operator*=(const T& val) const {
        return compound_assignment(val, [&](auto & lhs, const auto & rhs) { lhs *= rhs; });
    }
    template <typename T> constexpr void operator/=(const T& val) const {
        return compound_assignment(val, [&](auto & lhs, const auto & rhs) { lhs /= rhs; });
    }

    // Members
    Base _num;
    const static Base _den = std::numeric_limits<Base>::max();
};

// Literals
Unit<__UINT64_TYPE__> operator"" UU_64(long double val) { return Unit<__UINT64_TYPE__>(val); }
Unit<__UINT32_TYPE__> operator"" UU_32(long double val) { return Unit<__UINT32_TYPE__>(val); }
Unit<__UINT16_TYPE__> operator"" UU_16(long double val) { return Unit<__UINT16_TYPE__>(val); }
Unit<__UINT8_TYPE__>  operator"" UU_8(long double val)  { return Unit<__UINT8_TYPE__>(val); }
Unit<__INT64_TYPE__>  operator"" SU_64(long double val) { return Unit<__INT64_TYPE__>(val); }
Unit<__INT32_TYPE__>  operator"" SU_32(long double val) { return Unit<__INT32_TYPE__>(val); }
Unit<__INT16_TYPE__>  operator"" SU_16(long double val) { return Unit<__INT16_TYPE__>(val); }
Unit<__INT8_TYPE__>   operator"" SU_8(long double val)  { return Unit<__INT8_TYPE__>(val); }

} // namespace Dattatypes

#endif
