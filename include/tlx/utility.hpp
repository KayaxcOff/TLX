//
// Created by muham on 24.07.2026.
//

#ifndef TLX_UTILITY_HPP
#define TLX_UTILITY_HPP

#include <tlx/concepts.hpp>
#include <tlx/xr1.hpp>

namespace tlx {
    /**
     * @brief Returns the address of an object without invoking its operator&.
     *
     * @tparam Ty Type of the object.
     * @param ty Reference to the object.
     * @return Ty* Pointer to the object.
     */
    template<class Ty>
    [[nodiscard]]
    constexpr Ty* addressOf(Ty& ty) {
        return __builtin_addressof(ty);
    }
    template<class Ty>
    [[nodiscard]]
    constexpr const Ty* addressOf(const Ty& t) noexcept {
        return __builtin_addressof(t);
    }

    /**
     * @brief Checks if one type is derived from another at compile time.
     *
     * @tparam Ty1 Potential base class.
     * @tparam Ty2 Potential derived class.
     * @return true if `Ty2` is derived from `Ty1`.
     */
    template<class Ty1, class Ty2>
    [[nodiscard]]
    constexpr bool isBaseOf() {
        return __is_base_of(Ty1, Ty2);
    }

    /**
     * @brief Obtains a usable pointer to an object after its lifetime has been
     *        restarted in the same storage.
     *
     * This is typically used after placement new or other operations that
     * create a new object in previously occupied storage.
     *
     * @tparam Ty Type of the object.
     * @param t0 Pointer to the object.
     * @return Ty* Pointer to the newly created object.
     */
    template<class Ty>
    [[nodiscard]]
    constexpr Ty* launder(Ty* t0) noexcept {
        return ::__builtin_launder(t0);
    }

    /**
     * @brief Converts a lvalue or rvalue into a rvalue reference.
     *
     * @tparam Ty Type of the object.
     * @param ty Object to move.
     * @return remove_reference_t<Ty>&& Rvalue reference to the object.
     */
    template<class Ty>
    [[nodiscard]]
    constexpr remove_reference_t<Ty>&& move(Ty&& ty) {
        return static_cast<remove_reference_t<Ty>&&>(ty);
    }

    /**
     * @brief Swaps two objects efficiently using move semantics.
     *
     * This overload is constrained to types that are nothrow movable for
     * strong exception safety.
     *
     * @tparam T Type of the objects (must satisfy nothrow_movable).
     * @param t1 First object to swap.
     * @param t2 Second object to swap.
     */
    template<nothrow_movable T>
    constexpr void swap(T& t1, T& t2) noexcept {
        T t3 = ::tlx::move(t1);
        t1 = ::tlx::move(t2);
        t2 = ::tlx::move(t3);
    }

    template<class Ty, std::size_t N>
    constexpr void swap(Ty (&a)[N], Ty (&b)[N]) noexcept {
        for (size_t i = 0; i < N; ++i) {
            tlx::swap(a[i], b[i]);
        }
    }

    /**
     * @brief Converts an enum value to its underlying integer type.
     *
     * @tparam T Enumeration type (must satisfy enum_like).
     * @param t Enum value to convert.
     * @return underlying_type_t<T> The underlying integral value.
     */
    template<enum_like T>
    [[nodiscard]]
    constexpr underlying_type_t<T> ToUnderlying(T t) {
        return static_cast<underlying_type_t<T>>(t);
    }

    /**
     * @brief Perfect forwarding for lvalue references.
     *
     * @tparam Ty Type of the argument.
     * @param Arg Argument to forward.
     * @return Ty&& Forwarded argument.
     */
    template<class Ty>
    [[nodiscard]]
    constexpr Ty&& forward(remove_reference_t<Ty>& Arg) noexcept {
        return static_cast<Ty&&>(Arg);
    }
    /**
     * @brief Perfect forwarding for rvalue references.
     *
     * @tparam Ty Type of the argument.
     * @param Arg Argument to forward.
     * @return Ty&& Forwarded argument.
     */
    template <class Ty>
    [[nodiscard]]
    constexpr Ty&& forward(remove_reference_t<Ty>&& Arg) noexcept {
        static_assert(!::tlx::is_reference<Ty>, "bad forward call");
        return static_cast<Ty&&>(Arg);
    }

    /**
     * @brief Replaces the value of an object and returns the old value.
     *
     * @tparam Ty Type of the object.
     * @tparam U Type of the new value.
     * @param obj Object to modify.
     * @param newValue New value to assign.
     * @return Ty The old value of the object.
     */
    template<class Ty, class U = Ty>
    constexpr Ty exchange(Ty& obj, U&& newValue) {
        Ty output = ::tlx::move(obj);
        obj = ::tlx::forward<U>(newValue);
        return output;
    }

    /**
     * @brief A function object that returns its argument unchanged (identity function).
     *
     * Useful as a default projection or transformation function in algorithms.
     */
    struct identity {
        /**
         * @brief Applies the identity transformation.
         *
         * @tparam Ty Type of the value.
         * @param value Value to forward.
         * @return Ty&& The forwarded value.
         */
        template <class Ty>
        constexpr Ty&& operator()(Ty&& value) const noexcept {
            return ::tlx::forward<Ty>(value);
        }
    };

    /**
     * @brief Stores two values of the same type.
     *
     * This utility type is useful for representing a pair of related values,
     * such as ranges, coordinates, or other two-element aggregates.
     *
     * @tparam T Type of both stored values.
     */
    template<typename T>
    struct wide {
        T t1{};
        T t2{};

        wide() = default;
        wide(T t1, T t2) : t1(t1), t2(t2) {}
    };
} //namespace tlx

#endif //TLX_UTILITY_HPP