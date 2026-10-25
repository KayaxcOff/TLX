//
// Created by muham on 25.10.2026.
//

#ifndef TLX_FLOAT_HPP
#define TLX_FLOAT_HPP

#include <tlx/_detail/bit.hpp>
#ifdef TLX_CUDA
    #include <cuda_bf16.h>
    #include <cuda_fp16.h>
#endif //#ifdef TLX_CUDA
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
        TLX_HD
        bfloat16() = default;
        TLX_HD
        bfloat16(const float value) {
            this->m_value = float_to_bf16_bits(value);
        }
        #ifdef TLX_CUDA
            bfloat16(const __nv_bfloat16 value) {
                this->m_value = __bfloat16_as_ushort(value);
            }
        #endif //#ifdef TLX_CUDA
        explicit bfloat16(const std::uint16_t value) {
            this->m_value = value;
        }
        TLX_HD
        bfloat16(const bfloat16&) = default;
        TLX_HD
        bfloat16(bfloat16&&) noexcept = default;

        TLX_HD
        explicit operator float() const noexcept {
            return bf16_to_float_bits(this->m_value);
        }
        TLX_HD
        #ifdef TLX_CUDA
            TLX_HD
            explicit operator __nv_bfloat16() const {
                return __ushort_as_bfloat16(this->m_value);
            }
        #endif //#ifdef TLX_CUDA

        TLX_HD
        bfloat16 operator+(const bfloat16& other) const noexcept {
            #if defined(__CUDA_ARCH__)
                const auto t1 = static_cast<__nv_bfloat16>(*this);
                const auto t2 = static_cast<__nv_bfloat16>(other);
                return __hadd(t1, t2);
            #else //#if defined(__CUDA_ARCH__)
                const auto t1 = static_cast<float>(*this);
                const auto t2 = static_cast<float>(other);
                return t1 + t2;
            #endif //#if defined(__CUDA_ARCH__) #else
        }
        TLX_HD
        bfloat16 operator-(const bfloat16& other) const noexcept {
            #if defined(__CUDA_ARCH__)
                const auto t1 = static_cast<__nv_bfloat16>(*this);
                const auto t2 = static_cast<__nv_bfloat16>(other);
                return __hsub(t1, t2);
            #else //#if defined(__CUDA_ARCH__)
                const auto t1 = static_cast<float>(*this);
                const auto t2 = static_cast<float>(other);
                return t1 - t2;
            #endif //#if defined(__CUDA_ARCH__) #else
        }
        TLX_HD
        bfloat16 operator*(const bfloat16& other) const noexcept {
            #if defined(__CUDA_ARCH__)
                const auto t1 = static_cast<__nv_bfloat16>(*this);
                const auto t2 = static_cast<__nv_bfloat16>(other);
                return __hmul(t1, t2);
            #else //#if defined(__CUDA_ARCH__)
                const auto t1 = static_cast<float>(*this);
                const auto t2 = static_cast<float>(other);
                return t1 * t2;
            #endif //#if defined(__CUDA_ARCH__) #else
        }
        TLX_HD
        bfloat16 operator/(const bfloat16& other) const noexcept {
            #if defined(__CUDA_ARCH__)
                const auto t1 = static_cast<__nv_bfloat16>(*this);
                const auto t2 = static_cast<__nv_bfloat16>(other);
                return __hdiv(t1, t2);
            #else //#if defined(__CUDA_ARCH__)
                const auto t1 = static_cast<float>(*this);
                const auto t2 = static_cast<float>(other);
                return t1 * t2;
            #endif //#if defined(__CUDA_ARCH__) #else
        }

        TLX_HD
        bfloat16& operator+=(const bfloat16& other) noexcept {
            *this = *this + other;
            return *this;
        }
        TLX_HD
        bfloat16& operator-=(const bfloat16& other) noexcept {
            *this = *this - other;
            return *this;
        }
        TLX_HD
        bfloat16& operator*=(const bfloat16& other) noexcept {
            *this = *this * other;
            return *this;
        }
        TLX_HD
        bfloat16& operator/=(const bfloat16& other) noexcept {
            *this = *this / other;
            return *this;
        }

        TLX_HD
        bool operator==(const bfloat16& other) const noexcept {
            return static_cast<float>(*this) == static_cast<float>(other);
        }
        TLX_HD
        bool operator!=(const bfloat16& other) const noexcept {
            return static_cast<float>(*this) != static_cast<float>(other);
        }
        TLX_HD
        bool operator<(const bfloat16& other) const noexcept {
            return static_cast<float>(*this) < static_cast<float>(other);
        }
        TLX_HD
        bool operator>(const bfloat16& other) const noexcept {
            return static_cast<float>(*this) > static_cast<float>(other);
        }
        TLX_HD
        bool operator<=(const bfloat16& other) const noexcept {
            return static_cast<float>(*this) <= static_cast<float>(other);
        }
        TLX_HD
        bool operator>=(const bfloat16& other) const noexcept {
            return static_cast<float>(*this) >= static_cast<float>(other);
        }

        TLX_HD
        bfloat16& operator=(const bfloat16&) = default;
        TLX_HD
        bfloat16& operator=(bfloat16&&) noexcept = default;
    private:
        std::uint16_t m_value;
    };
    std::ostream& operator<<(std::ostream& os, const bfloat16& value);

    struct alignas(2) half {
        TLX_HD
        half() = default;
        TLX_HD
        half(const float value) {
            this->m_value = float_to_half_bits(value);
        }
        #ifdef TLX_CUDA
            half(const __nv_half value) {
                this->m_value = __half_as_ushort(value);
            }
        #endif //#ifdef TLX_CUDA
        TLX_HD
        explicit half(const std::uint16_t value) {
            this->m_value = value;
        }
        TLX_HD
        half(const half&) = default;
        TLX_HD
        half(half&&) noexcept = default;

        TLX_HD
        explicit operator float() const noexcept {
            return half_to_float_bits(this->m_value);
        }
        #ifdef TLX_CUDA
            TLX_HD
            explicit operator __nv_half() const noexcept {
                return __ushort_as_half(this->m_value);
            }
        #endif //#ifdef TLX_CUDA

        TLX_HD
        half operator+(const half& other) const noexcept {
            #if defined(__CUDA_ARCH__)
                const auto t1 = static_cast<__nv_half>(*this);
                const auto t2 = static_cast<__nv_half>(other);
                return __hadd(t1, t2);
            #else //#if defined(__CUDA_ARCH__)
                const auto t1 = static_cast<float>(*this);
                const auto t2 = static_cast<float>(other);
                return t1 + t2;
            #endif //#if defined(__CUDA_ARCH__)
        }
        TLX_HD
        half operator-(const half& other) const noexcept {
            #if defined(__CUDA_ARCH__)
                const auto t1 = static_cast<__nv_half>(*this);
                const auto t2 = static_cast<__nv_half>(other);
                return __hsub(t1, t2);
            #else //#if defined(__CUDA_ARCH__)
                const auto t1 = static_cast<float>(*this);
                const auto t2 = static_cast<float>(other);
                return t1 - t2;
            #endif //#if defined(__CUDA_ARCH__)
        }
        TLX_HD
        half operator*(const half& other) const noexcept {
            #if defined(__CUDA_ARCH__)
                const auto t1 = static_cast<__nv_half>(*this);
                const auto t2 = static_cast<__nv_half>(other);
                return __hmul(t1, t2);
            #else //#if defined(__CUDA_ARCH__)
                const auto t1 = static_cast<float>(*this);
                const auto t2 = static_cast<float>(other);
                return t1 * t2;
            #endif //#if defined(__CUDA_ARCH__)
        }
        TLX_HD
        half operator/(const half& other) const noexcept {
            #if defined(__CUDA_ARCH__)
                const auto t1 = static_cast<__nv_half>(*this);
                const auto t2 = static_cast<__nv_half>(other);
                return __hdiv(t1, t2);
            #else //#if defined(__CUDA_ARCH__)
                const auto t1 = static_cast<float>(*this);
                const auto t2 = static_cast<float>(other);
                return t1 / t2;
            #endif //#if defined(__CUDA_ARCH__)
        }

        TLX_HD
        half& operator+=(const half& other) noexcept {
            *this = *this + other;
            return *this;
        }
        TLX_HD
        half& operator-=(const half& other) noexcept {
            *this = *this - other;
            return *this;
        }
        TLX_HD
        half& operator*=(const half& other) noexcept {
            *this = *this * other;
            return *this;
        }
        TLX_HD
        half& operator/=(const half& other) noexcept {
            *this = *this / other;
            return *this;
        }

        TLX_HD
        bool operator==(const half& other) const noexcept {
            return static_cast<float>(*this) == static_cast<float>(other);
        }
        TLX_HD
        bool operator!=(const half& other) const noexcept {
            return static_cast<float>(*this) != static_cast<float>(other);
        }
        TLX_HD
        bool operator<(const half& other) const noexcept {
            return static_cast<float>(*this) < static_cast<float>(other);
        }
        TLX_HD
        bool operator>(const half& other) const noexcept {
            return static_cast<float>(*this) > static_cast<float>(other);
        }
        TLX_HD
        bool operator<=(const half& other) const noexcept {
            return static_cast<float>(*this) <= static_cast<float>(other);
        }
        TLX_HD
        bool operator>=(const half& other) const noexcept {
            return static_cast<float>(*this) >= static_cast<float>(other);
        }

        TLX_HD
        half& operator=(const half&) = default;
        TLX_HD
        half& operator=(half&&) noexcept = default;
    private:
        std::uint16_t m_value;
    };

    std::ostream& operator<<(std::ostream& os, const half& value);
} //namespace tlx

#endif //TLX_FLOAT_HPP