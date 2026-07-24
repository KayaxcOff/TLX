//
// Created by muham on 24.07.2026.
//

#ifndef TLX_CONCEPTS_HPP
#define TLX_CONCEPTS_HPP

namespace tlx {
    /**
     * @concept nothrow_movable
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
     * @concept enum_like
     * @brief Specifies that a type is an enumeration (scoped or unscoped).
     *
     * This concept is useful for constraints that should only apply to enum types.
     *
     * @tparam Ty The type to check.
     */
    template<class Ty>
    concept enum_like = __is_enum(Ty);
} //namespace tlx

#endif //TLX_CONCEPTS_HPP