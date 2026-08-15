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
        TLX_HD bfloat16() = default;
        TLX_HD bfloat16(const float value) {
            this->m_value = bit::float_to_bf16_bits(value);
        }
        #ifdef TLX_CUDA
            TLX_HD bfloat16(const __nv_bfloat16 value) {
                this->m_value = __bfloat16_as_ushort(value);
            }
        #endif //#ifdef TLX_CUDA
        TLX_HD bfloat16(const bfloat16&) = default;
        TLX_HD bfloat16(bfloat16&&) noexcept = default;

        TLX_HD operator float() const noexcept {
            return bit::bf16_to_float_bits(this->m_value);
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
            TLX_HD half(const __nv_half value) {
                this->m_value = __half_as_ushort(value);
            }
        #endif //#ifdef TLX_CUDA
        TLX_HD half(const half&) = default;
        TLX_HD half(half&&) noexcept = default;

        TLX_HD operator float() const noexcept {
            return bit::half_bits_to_float(this->m_value);
        }
        #ifdef TLX_CUDA
            TLX_HD operator __nv_half() const noexcept {
                return __ushort_as_half(this->m_value);
            }
        #endif //#ifdef TLX_CUDA

        TLX_HD half operator+(const half& other) const noexcept {
            #if defined(__CUDA_ARCH__)
                __nv_half a = static_cast<__nv_half>(*this);
                __nv_half b = static_cast<__nv_half>(other);
                return __hadd(a, b);
            #else //#if defined(__CUDA_ARCH__)
                return static_cast<float>(*this) + static_cast<float>(other);
            #endif //#if defined(__CUDA_ARCH__) #else
        }
        TLX_HD half operator-(const half& other) const noexcept {
            #if defined(__CUDA_ARCH__)
                __nv_half a = static_cast<__nv_half>(*this);
                __nv_half b = static_cast<__nv_half>(other);
                return __hsub(a, b);
            #else //#if defined(__CUDA_ARCH__)
                return static_cast<float>(*this) - static_cast<float>(other);
            #endif //#if defined(__CUDA_ARCH__) #else
        }
        TLX_HD half operator*(const half& other) const noexcept {
            #if defined(__CUDA_ARCH__)
                __nv_half a = static_cast<__nv_half>(*this);
                __nv_half b = static_cast<__nv_half>(other);
                return __hmul(a, b);
            #else //#if defined(__CUDA_ARCH__)
            return static_cast<float>(*this) * static_cast<float>(other);
            #endif //#if defined(__CUDA_ARCH__) #else
        }
        TLX_HD half operator/(const half& other) const noexcept {
            #if defined(__CUDA_ARCH__)
                __nv_half a = static_cast<__nv_half>(*this);
                __nv_half b = static_cast<__nv_half>(other);
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

    /**
     * @brief 16-bit signed quantized integer type.
     *
     * This type stores a signed 16-bit integer and promotes arithmetic
     * operations to 32-bit integers to reduce intermediate overflow risk.
     * It is aligned to 2 bytes and is usable on both host and device
     * (via the `TLX_HD` macro).
     */
    struct alignas(2) qint16 {
        TLX_HD qint16() = default;
        TLX_HD qint16(const std::int16_t value) noexcept {
            this->m_value = value;
        }
        TLX_HD qint16(const std::int32_t value) {
            this->m_value = static_cast<std::int16_t>(value);
        }
        TLX_HD qint16(const qint16&) = default;
        TLX_HD qint16(qint16&&) noexcept = default;

        TLX_HD operator std::int32_t() const noexcept {
            return this->m_value;
        }

        TLX_HD qint16 operator+(const qint16& other) const noexcept {
            return static_cast<std::int32_t>(*this) + static_cast<std::int32_t>(other);
        }
        TLX_HD qint16 operator-(const qint16& other) const noexcept {
            return static_cast<std::int32_t>(*this) - static_cast<std::int32_t>(other);
        }
        TLX_HD qint16 operator*(const qint16& other) const noexcept {
            return static_cast<std::int32_t>(*this) * static_cast<std::int32_t>(other);
        }
        TLX_HD qint16 operator/(const qint16& other) const noexcept {
            return static_cast<std::int32_t>(*this) / static_cast<std::int32_t>(other);
        }
        TLX_HD qint16& operator+=(const qint16& other) {
            *this = *this + other;
            return *this;
        }
        TLX_HD qint16& operator-=(const qint16& other) {
            *this = *this - other;
            return *this;
        }
        TLX_HD qint16& operator*=(const qint16& other) {
            *this = *this * other;
            return *this;
        }
        TLX_HD qint16& operator/=(const qint16& other) {
            *this = *this / other;
            return *this;
        }

        TLX_HD bool operator==(const qint16& other) const noexcept {
            return static_cast<std::int32_t>(*this) == static_cast<std::int32_t>(other);
        }
        TLX_HD bool operator!=(const qint16& other) const noexcept {
            return static_cast<std::int32_t>(*this) != static_cast<std::int32_t>(other);
        }
        TLX_HD bool operator<(const qint16& other) const noexcept {
            return static_cast<std::int32_t>(*this) < static_cast<std::int32_t>(other);
        }
        TLX_HD bool operator<=(const qint16& other) const noexcept {
            return static_cast<std::int32_t>(*this) <= static_cast<std::int32_t>(other);
        }
        TLX_HD bool operator>(const qint16& other) const noexcept {
            return static_cast<std::int32_t>(*this) > static_cast<std::int32_t>(other);
        }
        TLX_HD bool operator>=(const qint16& other) const noexcept {
            return static_cast<std::int32_t>(*this) >= static_cast<std::int32_t>(other);
        }

        TLX_HD qint16& operator=(const qint16&) = default;
        TLX_HD qint16& operator=(qint16&&) noexcept = default;
    private:
        std::int16_t m_value;
    };

    std::ostream& operator<<(std::ostream& os, const qint16& value);

    /**
     * @brief 8-bit signed quantized integer type.
     *
     * This type stores a signed 8-bit integer and promotes arithmetic
     * operations to 32-bit integers to reduce intermediate overflow risk.
     * It is aligned to 1 byte and is usable on both host and device
     * (via the `TLX_HD` macro).
     */
    struct alignas(1) qint8 {
        TLX_HD qint8() = default;
        TLX_HD qint8(const std::int8_t value) {
            this->m_value = value;
        }
        TLX_HD qint8(const std::int32_t value) {
            this->m_value = static_cast<std::int8_t>(value);
        }
        TLX_HD qint8(const qint8&) = default;
        TLX_HD qint8(qint8&&) noexcept = default;

        TLX_HD operator std::int32_t() const noexcept {
            return this->m_value;
        }

        TLX_HD qint8 operator+(const qint8& other) const noexcept {
            return static_cast<std::int32_t>(*this) + static_cast<std::int32_t>(other);
        }
        TLX_HD qint8 operator-(const qint8& other) const noexcept {
            return static_cast<std::int32_t>(*this) - static_cast<std::int32_t>(other);
        }
        TLX_HD qint8 operator*(const qint8& other) const noexcept {
            return static_cast<std::int32_t>(*this) * static_cast<std::int32_t>(other);
        }
        TLX_HD qint8 operator/(const qint8& other) const noexcept {
            return static_cast<std::int32_t>(*this) / static_cast<std::int32_t>(other);
        }

        TLX_HD qint8& operator+=(const qint8& other) {
            *this = *this + other;
            return *this;
        }
        TLX_HD qint8& operator-=(const qint8& other) {
            *this = *this - other;
            return *this;
        }
        TLX_HD qint8& operator*=(const qint8& other) {
            *this = *this * other;
            return *this;
        }
        TLX_HD qint8& operator/=(const qint8& other) {
            *this = *this / other;
            return *this;
        }

        TLX_HD bool operator==(const qint8& other) const noexcept {
            return static_cast<std::int32_t>(*this) == static_cast<std::int32_t>(other);
        }
        TLX_HD bool operator!=(const qint8& other) const noexcept {
            return static_cast<std::int32_t>(*this) != static_cast<std::int32_t>(other);
        }
        TLX_HD bool operator<(const qint8& other) const noexcept {
            return static_cast<std::int32_t>(*this) < static_cast<std::int32_t>(other);
        }
        TLX_HD bool operator<=(const qint8& other) const noexcept {
            return static_cast<std::int32_t>(*this) <= static_cast<std::int32_t>(other);
        }
        TLX_HD bool operator>(const qint8& other) const noexcept {
            return static_cast<std::int32_t>(*this) > static_cast<std::int32_t>(other);
        }
        TLX_HD bool operator>=(const qint8& other) const noexcept {
            return static_cast<std::int32_t>(*this) >= static_cast<std::int32_t>(other);
        }

        TLX_HD qint8& operator=(const qint8&) = default;
        TLX_HD qint8& operator=(qint8&&) noexcept = default;
    private:
        std::int8_t m_value;
    };

    std::ostream& operator<<(std::ostream& os, const qint8& value);

    /**
     * @brief 16-bit unsigned quantized integer type.
     *
     * This type stores an unsigned 16-bit integer. Arithmetic and comparison
     * operations are performed using the underlying 16-bit values.
     * It is aligned to 2 bytes and is usable on both host and device
     * (via the `TLX_HD` macro).
     */
    struct alignas(2) quint16 {
        TLX_HD quint16() = default;
        TLX_HD quint16(const std::uint16_t value) {
            this->m_value = value;
        }
        TLX_HD quint16(const std::int32_t value) {
            this->m_value = static_cast<std::uint16_t>(value);
        }
        TLX_HD quint16(const quint16&) = default;
        TLX_HD quint16(quint16&&) noexcept = default;

        TLX_HD operator std::uint32_t() const noexcept {
            return this->m_value;
        }

        TLX_HD quint16 operator+(const quint16& other) const noexcept {
            return static_cast<std::uint16_t>(*this) + static_cast<std::uint16_t>(other);
        }
        TLX_HD quint16 operator-(const quint16& other) const noexcept {
            return static_cast<std::uint16_t>(*this) - static_cast<std::uint16_t>(other);
        }
        TLX_HD quint16 operator*(const quint16& other) const noexcept {
            return static_cast<std::uint16_t>(*this) * static_cast<std::uint16_t>(other);
        }
        TLX_HD quint16 operator/(const quint16& other) const noexcept {
            return static_cast<std::uint16_t>(*this) / static_cast<std::uint16_t>(other);
        }

        TLX_HD quint16& operator+=(const quint16& other) {
            *this = *this + other;
            return *this;
        }
        TLX_HD quint16& operator-=(const quint16& other) {
            *this = *this - other;
            return *this;
        }
        TLX_HD quint16& operator*=(const quint16& other) {
            *this = *this * other;
            return *this;
        }
        TLX_HD quint16& operator/=(const quint16& other) {
            *this = *this / other;
            return *this;
        }

        TLX_HD bool operator==(const quint16& other) const noexcept {
            return static_cast<std::uint16_t>(*this) == static_cast<std::uint16_t>(other);
        }
        TLX_HD bool operator!=(const quint16& other) const noexcept {
            return static_cast<std::uint16_t>(*this) != static_cast<std::uint16_t>(other);
        }
        TLX_HD bool operator<(const quint16& other) const noexcept {
            return static_cast<std::uint16_t>(*this) < static_cast<std::uint16_t>(other);
        }
        TLX_HD bool operator<=(const quint16& other) const noexcept {
            return static_cast<std::uint16_t>(*this) <= static_cast<std::uint16_t>(other);
        }
        TLX_HD bool operator>(const quint16& other) const noexcept {
            return static_cast<std::uint16_t>(*this) > static_cast<std::uint16_t>(other);
        }
        TLX_HD bool operator>=(const quint16& other) const noexcept {
            return static_cast<std::uint16_t>(*this) >= static_cast<std::uint16_t>(other);
        }

        TLX_HD quint16& operator=(const quint16&) = default;
        TLX_HD quint16& operator=(quint16&&) noexcept = default;
    private:
        std::uint16_t m_value;
    };

    std::ostream& operator<<(std::ostream& os, const quint16& value);

    /**
     * @brief 8-bit unsigned quantized integer type.
     *
     * This type stores an unsigned 8-bit integer. Arithmetic and comparison
     * operations are performed using the underlying 8-bit values.
     * It is aligned to 1 byte and is usable on both host and device
     * (via the `TLX_HD` macro).
     */
    struct alignas(1) quint8 {
        TLX_HD quint8() = default;
        TLX_HD quint8(const std::uint8_t value) {
            this->m_value = value;
        }
        TLX_HD quint8(const std::int32_t value) {
            this->m_value = static_cast<std::uint8_t>(value);
        }
        TLX_HD quint8(const quint8&) = default;
        TLX_HD quint8(quint8&&) noexcept = default;

        TLX_HD operator std::uint32_t() const noexcept {
            return this->m_value;
        }

        TLX_HD quint8 operator+(const quint8& other) const noexcept {
            return static_cast<std::uint8_t>(*this) + static_cast<std::uint8_t>(other);
        }
        TLX_HD quint8 operator-(const quint8& other) const noexcept {
            return static_cast<std::uint8_t>(*this) - static_cast<std::uint8_t>(other);
        }
        TLX_HD quint8 operator*(const quint8& other) const noexcept {
            return static_cast<std::uint8_t>(*this) * static_cast<std::uint8_t>(other);
        }
        TLX_HD quint8 operator/(const quint8& other) const noexcept {
            return static_cast<std::uint8_t>(*this) / static_cast<std::uint8_t>(other);
        }

        TLX_HD quint8& operator+=(const quint8& other) {
            *this = *this + other;
            return *this;
        }
        TLX_HD quint8& operator-=(const quint8& other) {
            *this = *this - other;
            return *this;
        }
        TLX_HD quint8& operator*=(const quint8& other) {
            *this = *this * other;
            return *this;
        }
        TLX_HD quint8& operator/=(const quint8& other) {
            *this = *this / other;
            return *this;
        }

        TLX_HD bool operator==(const quint8& other) const noexcept {
            return static_cast<std::uint8_t>(*this) == static_cast<std::uint8_t>(other);
        }
        TLX_HD bool operator!=(const quint8& other) const noexcept {
            return static_cast<std::uint8_t>(*this) != static_cast<std::uint8_t>(other);
        }
        TLX_HD bool operator<(const quint8& other) const noexcept {
            return static_cast<std::uint8_t>(*this) < static_cast<std::uint8_t>(other);
        }
        TLX_HD bool operator<=(const quint8& other) const noexcept {
            return static_cast<std::uint8_t>(*this) <= static_cast<std::uint8_t>(other);
        }
        TLX_HD bool operator>(const quint8& other) const noexcept {
            return static_cast<std::uint8_t>(*this) > static_cast<std::uint8_t>(other);
        }
        TLX_HD bool operator>=(const quint8& other) const noexcept {
            return static_cast<std::uint8_t>(*this) >= static_cast<std::uint8_t>(other);
        }

        TLX_HD quint8& operator=(const quint8&) = default;
        TLX_HD quint8& operator=(quint8&&) noexcept = default;
    private:
        std::uint8_t m_value;
    };

    std::ostream& operator<<(std::ostream& os, const quint8& value);
} //namespace tlx

#endif //TLX_TYPES_HPP