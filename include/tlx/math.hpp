//
// Created by muham on 25.07.2026.
//

#ifndef TLX_MATH_HPP
#define TLX_MATH_HPP

#include <tlx/algorithm.hpp>

namespace tlx {
    /**
     * @brief Returns the sign of a value.
     *
     * Returns +1 if the value is positive, -1 if negative, and 0 if zero.
     * This is a branchless implementation.
     *
     * @tparam T Arithmetic type (must satisfy arithmetic_like).
     * @param value Value whose sign is to be determined.
     * @return T +1, -1, or 0 depending on the sign of the value.
     */
    template<arithmetic_like T>
    [[nodiscard]]
    constexpr T sign(const T& value) {
        return (T{} < value) - (value < T{});
    }
    /**
     * @brief Returns the absolute value of a number.
     *
     * @tparam T Arithmetic type (must satisfy arithmetic_like).
     * @param value Value whose absolute value is to be computed.
     * @return T Absolute value of the input.
     */
    template<arithmetic_like T>
    [[nodiscard]]
    constexpr T abs(T value) noexcept {
        return (value < 0) ? -value : value;
    }
    /**
     * @brief Performs linear interpolation between two values.
     *
     * Computes a + t * (b - a) with careful handling of edge cases
     * (different signs, t == 1, and overshoot protection).
     *
     * @tparam T Arithmetic type of the endpoints (must satisfy arithmetic_like).
     * @tparam U Arithmetic type of the interpolation factor (must satisfy arithmetic_like).
     * @param a Start value.
     * @param b End value.
     * @param t Interpolation factor (usually in [0, 1]).
     * @return auto Interpolated value.
     */
    template<arithmetic_like T, arithmetic_like U>
    [[nodiscard]]
    constexpr auto lerp(T a, T b, U t) noexcept {
        const T zero = static_cast<T>(0);
        const U one = static_cast<U>(1);

        if ((a <= zero && b >= zero) ||
            (a >= zero && b <= zero)) {
            return t * b + (one - t) * a;
            }

        if (t == one) {
            return b;
        }

        const auto x = a + t * (b - a);

        return (t > one) == (b > a)
            ? ::tlx::max(b, x)
            : ::tlx::min(b, x);
    }
    /**
     * @brief Clamps a value to a given range.
     *
     * Returns the value restricted to the closed interval [low, high].
     *
     * @tparam T Arithmetic type (must satisfy arithmetic_like).
     * @param value Value to clamp.
     * @param low Lower bound (inclusive).
     * @param high Upper bound (inclusive).
     * @return const T& Reference to the clamped value.
     */
    template<arithmetic_like T>
    [[nodiscard]]
    constexpr const T& clamp(const T& value, const T& low, const T& high) noexcept {
        return (value < low) ? low : (high < value) ? high : value;
    }
} //namespace tlx

#endif //TLX_MATH_HPP