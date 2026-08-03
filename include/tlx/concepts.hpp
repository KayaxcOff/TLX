//
// Created by muham on 24.07.2026.
//

#ifndef TLX_CONCEPTS_HPP
#define TLX_CONCEPTS_HPP

#include <tlx/types.hpp>
#include <type_traits>

namespace tlx {
    /**
     * @brief Specifies that a type is both nothrow move-constructible and nothrow move-assignable.
     *
     * A type satisfies `nothrow_movable` if it can be moved (both construction and assignment)
     * without throwing exceptions. This concept is particularly useful for optimization
     * and strong exception safety guarantees in containers and algorithms.
     *
     * @tparam Ty The type to check.
     */
    template<class Ty>
    concept nothrow_movable = __is_nothrow_constructible(Ty, Ty&&) && __is_nothrow_assignable(Ty&, Ty&&);

    /**
     * @brief Specifies that a type is an enumeration (scoped or unscoped).
     *
     * This concept is useful for constraints that should only apply to enum types.
     *
     * @tparam Ty The type to check.
     */
    template<class Ty>
    concept enum_like = __is_enum(Ty);

    /**
     * @brief Specifies that a type is an integral type.
     *
     * Equivalent to `std::is_integral_v<T>`.
     *
     * @tparam T The type to check.
     */
    template<typename T>
    concept integral = std::is_integral_v<T>;

    /**
     * @brief Specifies that a type is a floating-point type.
     *
     * Equivalent to `std::is_floating_point_v<T>`.
     *
     * @tparam T The type to check.
     */
    template<typename T>
    concept float_like = std::is_floating_point_v<T> || std::is_same_v<T, bfloat16> || std::is_same_v<T, half>;

    /**
     * @brief Specifies that a type is arithmetic (either integral or floating-point).
     *
     * This concept is satisfied if the type is either an integral type
     * or a floating-point type.
     *
     * @tparam T The type to check.
     */
    template<typename T>
    concept arithmetic_like = integral<T> || float_like<T> || std::is_same_v<T, qint16> || std::is_same_v<T, qint8> || std::is_same_v<T, quint16> || std::is_same_v<T, quint8>;

    /**
     * @brief Concept that checks if a type is nothrow destructible and constructible from given arguments.
     *
     * Combines nothrow destructibility with constructibility from the provided argument types.
     *
     * @tparam Ty   The type to be constructed.
     * @tparam Args The argument types used for construction.
     */
    template<class Ty, class... Args>
    concept constructible_from = __is_nothrow_destructible(Ty) && __is_constructible(Ty, Args...);

    /**
     * @brief Specifies that one type is derived from another.
     *
     * This concept is satisfied if `Ty1` is publicly and unambiguously
     * derived from `Ty2` (or is the same as `Ty2`). It is an alias for
     * `std::derived_from<Ty1, Ty2>`.
     *
     * @tparam Ty1 The potential derived type.
     * @tparam Ty2 The potential base type.
     */
    template<class Ty1, class Ty2>
    concept extend = std::derived_from<Ty1, Ty2>;
} //namespace tlx

#endif //TLX_CONCEPTS_HPP