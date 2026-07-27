//
// Created by muham on 26.07.2026.
//

#ifndef TLX_BIT_HPP
#define TLX_BIT_HPP

#include <tlx/macros.hpp>
#include <cstdint>
#ifdef TLX_CUDA
    #include <cuda/std/bit>
    using cuda::std::bit_cast;
#else //#ifdef TLX_CUDA
    #include <bit>
    using std::bit_cast;
#endif //#ifdef TLX_CUDA #€lse

namespace tlx::bit {
    /**
     * @brief Converts a single-precision float to a 16-bit half-precision bit pattern.
     *
     * Performs IEEE 754 compliant conversion from float32 to float16, including
     * proper handling of special values (NaN, Infinity), overflow, underflow,
     * and round-to-nearest-even.
     *
     * @param value Input single-precision floating-point value.
     * @return std::uint16_t 16-bit representation of the half-precision value.
     */
    [[nodiscard]]
    TLX_HD inline std::uint16_t float_to_half_bits(const float value) noexcept {
        const auto bits = bit_cast<std::uint32_t>(value);

        const std::uint32_t sign = (bits >> 16) & 0x8000u;
        auto exp = static_cast<std::int32_t>((bits >> 23) & 0xFFu);
        std::uint32_t mantissa = bits & 0x7FFFFFu;

        if (exp == 255) {
            if (mantissa == 0) {
                return static_cast<std::uint16_t>(sign | 0x7C00u);
            }

            return static_cast<std::uint16_t>(
                sign | 0x7C00u | (mantissa >> 13) | 1u
            );
        }

        exp = exp - 127 + 15;

        if (exp >= 31) {
            return static_cast<std::uint16_t>(sign | 0x7C00u);
        }

        if (exp <= 0) {
            if (exp < -10) {
                return static_cast<std::uint16_t>(sign);
            }

            mantissa |= 0x800000u;

            const auto shift = static_cast<std::uint32_t>(14 - exp);

            std::uint32_t result = mantissa >> shift;

            const std::uint32_t remainder = mantissa & ((1u << shift) - 1);
            const std::uint32_t halfway = 1u << (shift - 1);

            if (remainder > halfway || (remainder == halfway && (result & 1))) {
                ++result;
            }

            return static_cast<std::uint16_t>(sign | result);
        }


        mantissa += 0x1000u;

        if (mantissa & 0x800000u) {
            mantissa = 0;
            ++exp;

            if (exp >= 31) {
                return static_cast<std::uint16_t>(sign | 0x7C00u);
            }
        }

        return static_cast<std::uint16_t>(
            sign | (static_cast<std::uint32_t>(exp) << 10) | (mantissa >> 13)
        );
    }

    /**
     * @brief Converts a 16-bit half-precision bit pattern to a single-precision float.
     *
     * Performs IEEE 754 compliant conversion from float16 to float32, correctly
     * handling normal numbers, denormals, zeros, infinities and NaNs.
     *
     * @param bits 16-bit half-precision bit pattern.
     * @return float Equivalent single-precision floating-point value.
     */
    [[nodiscard]]
    TLX_HD inline float half_bits_to_float(const std::uint16_t bits) noexcept {
        const std::uint32_t sign = (bits & 0x8000u) << 16;
        std::uint32_t exp = (bits >> 10) & 0x1Fu;
        std::uint32_t mantissa = bits & 0x03FFu;

        std::uint32_t result;

        if (exp == 0) {
            if (mantissa == 0) {
                result = sign;
            } else {
                exp = 127 - 15 + 1;

                while ((mantissa & 0x400u) == 0) {
                    mantissa <<= 1;
                    --exp;
                }

                mantissa &= 0x3FFu;

                result = sign | (exp << 23) | (mantissa << 13);
            }
        } else if (exp == 31) {
            result =
                sign |
                0x7F800000u |
                (mantissa << 13);
        } else {
            exp = exp + (127 - 15);

            result = sign | (exp << 23) | (mantissa << 13);
        }

        return bit_cast<float>(result);
    }

    /**
     * @brief Converts a single-precision float to a 16-bit bfloat16 bit pattern.
     *
     * Performs a fast conversion from IEEE 754 float32 to bfloat16 by
     * rounding to nearest even and truncating the lower 16 bits of the
     * mantissa. Special values (NaN, Infinity) are handled correctly
     * by the rounding step.
     *
     * @param value Input single-precision floating-point value.
     * @return std::uint16_t 16-bit representation of the bfloat16 value.
     */
    [[nodiscard]]
    TLX_HD TLX_INLINE std::uint16_t float_to_bf16_bits(const float value) noexcept {
        auto f_bits = bit_cast<std::uint32_t>(value);
        f_bits += 0x7FFFu + ((f_bits >> 16) & 1u);
        return static_cast<std::uint16_t>(f_bits >> 16);
    }

    /**
     * @brief Converts a 16-bit bfloat16 bit pattern to a single-precision float.
     *
     * Reconstructs a float32 value by shifting the 16-bit bfloat16 pattern
     * into the upper half of a 32-bit word (zero-extending the mantissa).
     *
     * @param value 16-bit bfloat16 bit pattern.
     * @return float Equivalent single-precision floating-point value.
     */
    [[nodiscard]]
    TLX_HD TLX_INLINE float bf16_to_float_bits(const std::uint16_t value) noexcept {
        const std::uint32_t f_bits = static_cast<std::uint32_t>(value) << 16;
        return bit_cast<float>(f_bits);
    }
} //namespace tlx::bit

#endif //TLX_BIT_HPP