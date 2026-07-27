//
// Created by muham on 26.07.2026.
//

#ifndef TLX_TYPES_HPP
#define TLX_TYPES_HPP

#include <tlx/macros.hpp>
#include <tlx/bit.hpp>
#ifdef TLX_CUDA
    #include <cuda_bf16.h>
    #include <cuda_fp16.h>
    #include <cuda/std/bit>
    using cuda::std::bit_cast;
#else //#ifdef TLX_CUDA
    #include <bit>
    using std::bit_cast;
#endif //#ifdef TLX_CUDA #else
#include <ostream>

namespace tlx {
    /**
     * @brief 16-bit Brain Floating Point type.
     *
     * This type stores a 16-bit floating-point value in the bfloat16 format
     * (1 sign bit, 8 exponent bits, 7 mantissa bits). It provides seamless
     * conversion to/from `float` and (when compiled with CUDA) `__nv_bfloat16`.
     * All arithmetic and comparison operators are supported.
     *
     * The type is aligned to 2 bytes and is usable in both host and device code
     * (via the `TLX_HD` macro).
     */
    struct alignas(2) bfloat16 {
        TLX_HD constexpr bfloat16() = default;
        TLX_HD constexpr bfloat16(const float value) {
            auto f_bits = bit_cast<std::uint32_t>(value);
            f_bits += 0x7FFFu + ((f_bits >> 16) & 1u);
            this->m_value = static_cast<std::uint16_t>(f_bits >> 16);
        }
        #ifdef TLX_CUDA
            TLX_HD bfloat16(const __nv_bfloat16 value) {
                this->m_value = __bfloat16_as_ushort(value);
            }
        #endif //#ifdef TLX_CUDA
        TLX_HD constexpr bfloat16(const bfloat16&) = default;
        TLX_HD constexpr bfloat16(bfloat16&&) noexcept = default;

        TLX_HD constexpr operator float() const noexcept {
            const std::uint32_t f_bits = static_cast<std::uint32_t>(this->m_value) << 16;
            return bit_cast<float>(f_bits);
        }
        #ifdef TLX_CUDA
            TLX_HD operator __nv_bfloat16() const {
                return __ushort_as_bfloat16(this->m_value);
            }
        #endif //#ifdef TLX_CUDA

        TLX_HD bfloat16 operator+(const bfloat16& other) const noexcept {
            #if defined(__CUDA_ARCH__)
                __nv_bfloat16 a = static_cast<__nv_bfloat16>(*this);
                __nv_bfloat16 b = static_cast<__nv_bfloat16>(other);
                return __hadd(a, b);
            #else //#if defined(__CUDA_ARCH__)
                return static_cast<float>(*this) + static_cast<float>(other);
            #endif //#if defined(__CUDA_ARCH__) #else
        }
        TLX_HD bfloat16 operator-(const bfloat16& other) const noexcept {
                #if defined(__CUDA_ARCH__)
                    __nv_bfloat16 a = static_cast<__nv_bfloat16>(*this);
                    __nv_bfloat16 b = static_cast<__nv_bfloat16>(other);
                    return __hsub(a, b);
                #else //#if defined(__CUDA_ARCH__)
                    return static_cast<float>(*this) - static_cast<float>(other);
                #endif //#if defined(__CUDA_ARCH__) #else
                }
        TLX_HD bfloat16 operator*(const bfloat16& other) const noexcept {
                #if defined(__CUDA_ARCH__)
                    __nv_bfloat16 a = static_cast<__nv_bfloat16>(*this);
                    __nv_bfloat16 b = static_cast<__nv_bfloat16>(other);
                    return __hmul(a, b);
                #else //#if defined(__CUDA_ARCH__)
                    return static_cast<float>(*this) * static_cast<float>(other);
                #endif //#if defined(__CUDA_ARCH__) #else
            }
        TLX_HD bfloat16 operator/(const bfloat16& other) const noexcept {
                #if defined(__CUDA_ARCH__)
                    __nv_bfloat16 a = static_cast<__nv_bfloat16>(*this);
                    __nv_bfloat16 b = static_cast<__nv_bfloat16>(other);
                    return __hdiv(a, b);
                #else //#if defined(__CUDA_ARCH__)
                    return static_cast<float>(*this) / static_cast<float>(other);
                #endif //#if defined(__CUDA_ARCH__) #else
            }

        TLX_HD bfloat16& operator+=(const bfloat16& other) {
                *this = *this + other;
                return *this;
            }
        TLX_HD bfloat16& operator-=(const bfloat16& other) {
                *this = *this - other;
                return *this;
            }
        TLX_HD bfloat16& operator*=(const bfloat16& other) {
                *this = *this * other;
                return *this;
            }
        TLX_HD bfloat16& operator/=(const bfloat16& other) {
                *this = *this / other;
                return *this;
            }

        TLX_HD bool operator==(const bfloat16& other) const {
                return static_cast<float>(*this) == static_cast<float>(other);
            }
        TLX_HD bool operator!=(const bfloat16& other) const {
                return !(*this == other);
            }
        TLX_HD bool operator<(const bfloat16& other) const {
                return static_cast<float>(*this) < static_cast<float>(other);
            }
        TLX_HD bool operator<=(const bfloat16& other) const {
                return static_cast<float>(*this) <= static_cast<float>(other);
            }
        TLX_HD bool operator>(const bfloat16& other) const {
                return static_cast<float>(*this) > static_cast<float>(other);
            }
        TLX_HD bool operator>=(const bfloat16& other) const {
                return static_cast<float>(*this) >= static_cast<float>(other);
            }

        TLX_HD bfloat16& operator=(const bfloat16& other) = default;
        TLX_HD bfloat16& operator=(bfloat16&& other) noexcept = default;
    private:
        std::uint16_t m_value;
    };

    std::ostream& operator<<(std::ostream& os, const bfloat16& value);

    /**
     * @brief 16-bit IEEE 754 half-precision floating-point type (binary16).
     *
     * This type stores a 16-bit floating-point value in the IEEE 754 half-precision
     * format (1 sign bit, 5 exponent bits, 10 mantissa bits). It provides seamless
     * conversion to/from `float` and (when compiled with CUDA) `__half`.
     * All arithmetic and comparison operators are supported.
     *
     * The type is aligned to 2 bytes and is usable in both host and device code
     * (via the `TLX_HD` macro).
     */
    struct alignas(2) half {
        TLX_HD half() = default;
        TLX_HD half(const float value) {
            this->m_value = bit::float_to_half_bits(value);
        }
        #ifdef TLX_CUDA
            TLX_HD half(const __half value) {
                this->m_value = __half_as_ushort(value);
            }
        #endif //#ifdef TLX_CUDA
        TLX_HD half(const half&) = default;
        TLX_HD half(half&&) noexcept = default;

        TLX_HD operator float() const noexcept {
            return bit::half_bits_to_float(this->m_value);
        }
        #ifdef TLX_CUDA
            TLX_HD operator __half() const noexcept {
                return __ushort_as_half(this->m_value);
            }
        #endif //#ifdef TLX_CUDA

        TLX_HD half operator+(const half& other) const noexcept {
            #if defined(__CUDA_ARCH__)
                __nv_half a = static_cast<__nv_half>(other);
                __nv_half b = static_cast<__nv_half>(*this);
                return __hadd(a, b);
            #else //#if defined(__CUDA_ARCH__)
                return static_cast<float>(*this) + static_cast<float>(other);
            #endif //#if defined(__CUDA_ARCH__) #else
        }
        TLX_HD half operator-(const half& other) const noexcept {
            #if defined(__CUDA_ARCH__)
                __nv_half a = static_cast<__nv_half>(other);
                __nv_half b = static_cast<__nv_half>(*this);
                return __hsub(a, b);
            #else //#if defined(__CUDA_ARCH__)
                return static_cast<float>(*this) - static_cast<float>(other);
            #endif //#if defined(__CUDA_ARCH__) #else
        }
        TLX_HD half operator*(const half& other) const noexcept {
            #if defined(__CUDA_ARCH__)
                __nv_half a = static_cast<__nv_half>(other);
                __nv_half b = static_cast<__nv_half>(*this);
                return __hmul(a, b);
            #else //#if defined(__CUDA_ARCH__)
            return static_cast<float>(*this) * static_cast<float>(other);
            #endif //#if defined(__CUDA_ARCH__) #else
        }
        TLX_HD half operator/(const half& other) const noexcept {
            #if defined(__CUDA_ARCH__)
                __nv_half a = static_cast<__nv_half>(other);
                __nv_half b = static_cast<__nv_half>(*this);
                return __hdiv(a, b);
            #else //#if defined(__CUDA_ARCH__)
                return static_cast<float>(*this) / static_cast<float>(other);
            #endif //#if defined(__CUDA_ARCH__) #else
        }

        TLX_HD half& operator+=(const half& other) {
            *this = *this + other;
            return *this;
        }
        TLX_HD half& operator-=(const half& other) {
            *this = *this - other;
            return *this;
        }
        TLX_HD half& operator*=(const half& other) {
            *this = *this * other;
            return *this;
        }
        TLX_HD half& operator/=(const half& other) {
            *this = *this / other;
            return *this;
        }

        TLX_HD bool operator==(const half& other) const noexcept {
            return static_cast<float>(*this) == static_cast<float>(other);
        }
        TLX_HD bool operator!=(const half& other) const noexcept {
            return static_cast<float>(*this) != static_cast<float>(other);
        }
        TLX_HD bool operator<(const half& other) const noexcept {
            return static_cast<float>(*this) < static_cast<float>(other);
        }
        TLX_HD bool operator<=(const half& other) const noexcept {
            return static_cast<float>(*this) <= static_cast<float>(other);
        }
        TLX_HD bool operator>(const half& other) const noexcept {
            return static_cast<float>(*this) > static_cast<float>(other);
        }
        TLX_HD bool operator>=(const half& other) const noexcept {
            return static_cast<float>(*this) >= static_cast<float>(other);
        }

        TLX_HD half& operator=(const half&) = default;
        TLX_HD half& operator=(half&&) noexcept = default;
    private:
        std::uint16_t m_value;
    };

    std::ostream& operator<<(std::ostream& os, const half& value);
} //namespace tlx

#endif //TLX_TYPES_HPP