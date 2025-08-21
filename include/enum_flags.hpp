#pragma once
// === HEADER ONLY ===

#include <type_traits>
#include <cstdint>


namespace dattatypes {

    template <typename T>
    constexpr bool is_scoped_enum_v = std::is_enum_v<T> && !std::is_convertible_v<T, std::underlying_type_t<T>>;

    // Bitwise Enum Operators
    template <typename T>
    constexpr std::enable_if_t<is_scoped_enum_v<T>, T>
    operator|(T a, T b) {
        using U = std::underlying_type_t<T>;
        return static_cast<T>(static_cast<U>(a) | static_cast<U>(b));
    }
    template <typename T>
    constexpr std::enable_if_t<is_scoped_enum_v<T>, T>
    operator&(const T a, const T b) {
        using U = std::underlying_type_t<T>;
        return T(U(a) & U(b));
    };
    template <typename T>
    constexpr std::enable_if_t<is_scoped_enum_v<T>, T>
    operator^(const T a, const T b) {
        static_assert(is_scoped_enum_v<T>, "T must be a scoped enum (enum class)");
        using U = std::underlying_type_t<T>;
        return T(U(a) ^ U(b));
    };

    // Template Class for using Enums as Flags.
    template <typename T>
    class Flags {
        static_assert(is_scoped_enum_v<T>, "T must be a scoped enum (enum class)");

    protected:
        using U = std::underlying_type_t<T>;
        T _data;

    public:
        constexpr Flags() : _data(T(0)) {}
        constexpr Flags(T value) : _data(value) {}

        // Flag Operations
        constexpr bool check(T other) const { return (U(_data) & U(other)) != 0; }
        constexpr void set(T other) { _data = T(U(_data) | U(other)); }
        constexpr void unset(T other) { _data = T(U(_data) & ~U(other)); }
        constexpr void flip(T other) { _data = T(U(_data) ^ U(other)); }

        // Other Operators
        constexpr Flags &operator=(T other) { _data = other; return *this; }
        operator T() const { return _data; };
        constexpr U underlying() const { return U(_data); };

        // (De)Serialization
        template <class Archive>
        void serialize(Archive &ar) { ar(_data); }
    };

    /**
     * Template Class for using Enums as Flags.
     * Stores either Enum or Flag.
     * The USE_FLAGS-flag should be located at the MSB.
     */
    template <typename T>
    class FlagsOrValue : public Flags<T> {
        static_assert(requires { T::USE_FLAGS; }, "T must define USE_FLAGS");

    public:
        using Flags<T>::Flags;

        constexpr bool uses_flags() const { return this->check(T::USE_FLAGS); }

        // (De)Serialization
        template <class Archive>
        void serialize(Archive &ar) { ar(this->_data); }
    };

    /**
     * Template Class for using Enums as Flags.
     * Stores both Enum and Flag.
     * Flags should be located at the MSBs.
     */
    template <typename T>
    class FlagsAndValue : public Flags<T> {
        static_assert(requires { T::NUMBER_OF_FLAGS; }, "T must define NUMBER_OF_FLAGS");

        using U = std::underlying_type_t<T>;
        static constexpr U flags_width = U(T::NUMBER_OF_FLAGS);
        static constexpr U enum_width = sizeof(U) * 8 - flags_width;
        static constexpr U enum_mask = (1 << enum_width) - 1;

    public:
        using Flags<T>::Flags;

        constexpr T get_enum() const { return T(U(this->_data) & enum_mask); };
        constexpr void set_enum(const T value) { this->_data = T(U(this->_data) & ~enum_mask | U(value)); };
        constexpr void clear_flags() { this->_data = get_enum(); };

        // (De)Serialization
        template <class Archive>
        void serialize(Archive &ar) { ar(this->_data); }
    };



}; // namespace dattatypes
