//
// Created by muham on 25.07.2026.
//

#ifndef TLX_ALGORITHM_HPP
#define TLX_ALGORITHM_HPP

#include <tlx/concepts.hpp>

namespace tlx {
    /**
     * @brief Returns the greater of two values.
     *
     * @tparam T Arithmetic type (must satisfy arithmetic_like).
     * @param t1 First value.
     * @param t2 Second value.
     * @return const T& Reference to the greater value.
     */
    template<arithmetic_like T>
    [[nodiscard]]
    constexpr T& max(const T& t1, const T& t2) {
        return t1 > t2 ? t1 : t2;
    }
    /**
     * @brief Returns the lesser of two values.
     *
     * @tparam T Arithmetic type (must satisfy arithmetic_like).
     * @param t1 First value.
     * @param t2 Second value.
     * @return const T& Reference to the lesser value.
     */
    template<arithmetic_like T>
    [[nodiscard]]
    constexpr T& min(const T& t1, const T& t2) {
        return t1 < t2 ? t1 : t2;
    }

    /**
     * @brief Checks whether a value lies within a closed range [low, high].
     *
     * @tparam T Arithmetic type (must satisfy arithmetic_like).
     * @param value Value to test.
     * @param low Lower bound (inclusive).
     * @param high Upper bound (inclusive).
     * @return true if low ≤ value ≤ high.
     */
    template<arithmetic_like T>
    [[nodiscard]]
    constexpr bool in_range(const T& value, const T& low, const T& high) {
        return !(value < low) && !(high < value);
    }

    /**
     * @brief Checks whether a value is equal to zero.
     *
     * @tparam T Arithmetic type (must satisfy arithmetic_like).
     * @param t Value to test.
     * @return true if the value is zero.
     */
    template<arithmetic_like T>
    [[nodiscard]]
    constexpr bool isZero(const T& t) {
        return t == T{};
    }
    /**
     * @brief Checks whether a value is strictly positive.
     *
     * @tparam T Arithmetic type (must satisfy arithmetic_like).
     * @param t Value to test.
     * @return true if the value is greater than zero.
     */
    template<arithmetic_like T>
    [[nodiscard]]
    constexpr bool isPositive(const T& t) {
        return t > T{};
    }
    /**
     * @brief Checks whether a value is strictly negative.
     *
     * @tparam T Arithmetic type (must satisfy arithmetic_like).
     * @param t Value to test.
     * @return true if the value is less than zero.
     */
    template<arithmetic_like T>
    [[nodiscard]]
    constexpr bool isNegative(const T& t) {
        return t < T{};
    }
    /**
     * @brief Checks whether an integral value lies within a closed range [start, end].
     *
     * @tparam T Integral type (must satisfy integral_t).
     * @param value Value to test.
     * @param start Lower bound (inclusive).
     * @param end Upper bound (inclusive).
     * @return true if start ≤ value ≤ end.
     */
    template<integral_t T>
    [[nodiscard]]
    constexpr bool between(const T& value, const T& start, const T& end) noexcept {
        return start <= value && value <= end;
    }
} //namespace tlx

#endif //TLX_ALGORITHM_HPP