#pragma once

#include <type_traits>
#include <cmath>
#include <stdexcept>



#include "debug.hpp"
static auto src = "Prec:TEST";




#include <prec.hpp>

namespace Dattatypes {


    typedef Prec<int8_t, -2>     prec8;     // 8-bit signed 1/4 precision number. Range: [-32, 31.75]
    typedef Prec<int16_t, -4>    prec16;    // 16-bit signed 1/16 precision number. Range: [-2048, 2047.9375]
    typedef Prec<int32_t, -8>    prec32;    // 32-bit signed 1/256 precision number. Range: [-8388608, 8388608)
    typedef Prec<int64_t, -16>   prec64;    // 64-bit signed 1/65536 precision number. Range: [-140737488355328, 140737488355328)
    typedef Prec<u_int8_t, -2>   u_prec8;   // 8-bit unsigned 1/4 precision number. Range: [0, 63.75]
    typedef Prec<u_int16_t, -4>  u_prec16;  // 16-bit unsigned 1/16 precision number. Range: [0, 4095.9375]
    typedef Prec<u_int32_t, -8>  u_prec32;  // 32-bit unsigned 1/256 precision number. Range: [0, 16777216)
    typedef Prec<u_int64_t, -16> u_prec64;  // 64-bit unsigned 1/65536 precision number. Range: [0, 281474976710656)

    typedef Prec<int8_t, -7>     unit8;     // 8-bit signed 1/128 precision unit number. Range: [-1.0, 1.0)
    typedef Prec<int16_t, -15>   unit16;    // 16-bit signed 1/32768 precision unit number. Range: [-1.0, 1.0)
    typedef Prec<int32_t, -31>   unit32;    // 32-bit signed 1/2147483648 precision unit number. Range: [-1.0, 1.0)
    typedef Prec<int64_t, -63>   unit64;    // 64-bit signed 1/9223372036854775808 precision unit number. Range: [-1.0, 1.0)
    typedef Prec<u_int8_t, -8>   u_unit8;   // 8-bit unsigned 1/256 precision unit number. Range: [0.0, 1.0)
    typedef Prec<u_int16_t, -16> u_unit16;  // 16-bit unsigned 1/65536 precision unit number. Range: [0.0, 1.0)
    typedef Prec<u_int32_t, -32> u_unit32;  // 32-bit unsigned 1/4294967296 precision unit number. Range: [0.0, 1.0)
    typedef Prec<u_int64_t, -64> u_unit64;  // 64-bit unsigned 1/18446744073709551616 precision unit number. Range: [0.0, 1.0)

    typedef Prec<int8_t, -6>     angle8;    // 8-bit signed 1/64 precision number. Range: [-2.0, 2.0)
    typedef Prec<int16_t, -14>   angle16;   // 16-bit signed 1/16384 precision number. Range: [-2.0, 2.0)
    typedef Prec<int32_t, -30>   angle32;   // 32-bit signed 1/1073741824 precision number. Range: [-2.0, 2.0)
    typedef Prec<int64_t, -62>   angle64;   // 64-bit signed 1/4611686018427387904 precision number. Range: [-2.0, 2.0)
    typedef Prec<u_int8_t, -7>   prob8;     // 8-bit unsigned 1/128 precision number. Range: [0.0, 2.0)
    typedef Prec<u_int16_t, -15> prob16;    // 16-bit unsigned 1/32768 precision number. Range: [0.0, 2.0)
    typedef Prec<u_int32_t, -31> prob32;    // 32-bit unsigned 1/2147483648 precision number. Range: [0.0, 2.0)
    typedef Prec<u_int64_t, -63> prob64;    // 64-bit unsigned 1/9223372036854775808 precision number. Range: [0.0, 2.0)


    #define ASSERT_PREC(T) static_assert( \
        is_prec<T>::value || \
        "T should be of type Prec");

    #define ASSERT_ANGLE(T) ASSERT_PREC(T) static_assert( \
        std::is_signed_v<typename T::value_type> && \
        (T::_n == 8 * sizeof(typename T::value_type) - 2), \
        "Type is not a Prec-based angle. I.e.: where the range is [-2.0, 2.0)." );

    #define ASSERT_PROBABILITY(T) ASSERT_PREC(T) static_assert( \
        std::is_unsigned_v<typename T::value_type> && \
        (T::_n == 8 * sizeof(typename T::value_type) - 1), \
        "Type is not a Prec-based probability. I.e.: where the range is [0.0, 2.0)." );


    /**
     * Square Root for Prec numbers.
     * Using Dampened Newton-Raphson Iteration.
     *
     * TODO: It is currently rather expensive
     */
    template<typename T>
    constexpr T sqrt(const T& prec, int iterations = 32) {
        ASSERT_PREC(T)
        if constexpr (std::is_constant_evaluated()) {
            if (prec._data < 0) return T(-1);
        } else
            if (prec._data < 0) throw std::runtime_error("sqrt input should be non-negative");

        u_int64_t S = u_int64_t(prec._data);
        u_int64_t x_n = S >> 1;
        int x_nm1;
        int shift = -T::_n;
        int t = 1 << -shift;

        LOG_DEBUG("S = {} : {}", S >> 8, S);

        // Dampened Newton-Raphson Iteration
        while (iterations-- > 0 && x_nm1 != x_n) {
            x_nm1 = x_n;
            x_n = ( (x_n+1) + ((S / (x_n)) << -shift)) >> 1;
            LOG_DEBUG("x_n = {} : {}", float(x_n) / t, x_n);
        }

        T res;
        res._data = x_n;

        return res;
    }



    // TODO(): Implement Trigonometric functions
    // template <typename Angle, typename Slope>
    // constexpr Slope sin(const Angle& value) {
    //     ASSERT_ANGLE(A)
    //     return 1;
    // }
    // template <typename Angle, typename Slope>
    // constexpr Slope cos(const Angle& value) {
    //     ASSERT_ANGLE(A)
    //     return 1;
    // }
    //     template <typename Slope, typename Angle>
    // constexpr Angle asin(const Slope& value) {
    //     ASSERT_ANGLE(A)
    //     return 1;
    // }
    // template <typename Slope, typename Angle>
    // constexpr Angle acos(const Slope& value) {
    //     ASSERT_ANGLE(A)
    //     return 1;
    // }



}; // namespace Dattatypes
