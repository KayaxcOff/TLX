//
// Created by muham on 1.11.2026.
//

#ifndef TLX_QUANTIZE_TYPES_HPP
#define TLX_QUANTIZE_TYPES_HPP

#include <tlx/_detail/flags.hpp>
#include <ostream>

namespace tlx {
    struct alignas(1) qint8 {
        TLX_HD
        qint8() = default;
        TLX_HD
        explicit qint8(const std::int8_t value) {
            this->m_value = value;
        }
        TLX_HD
        qint8(const std::int32_t value) {
            this->m_value = static_cast<std::int8_t>(value);
        }
        TLX_HD
        qint8(const qint8&) = default;
        TLX_HD
        qint8(qint8&&) noexcept = default;

        explicit operator std::int32_t() const {
            return this->m_value;
        }

        TLX_HD
        qint8 operator+(const qint8& other) const {
            return static_cast<std::int32_t>(*this) + static_cast<std::int32_t>(other);
        }
        TLX_HD
        qint8 operator-(const qint8& other) const {
            return static_cast<std::int32_t>(*this) - static_cast<std::int32_t>(other);
        }
        TLX_HD
        qint8 operator*(const qint8& other) const {
            return static_cast<std::int32_t>(*this) * static_cast<std::int32_t>(other);
        }
        TLX_HD
        qint8 operator/(const qint8& other) const {
            return static_cast<std::int32_t>(*this) / static_cast<std::int32_t>(other);
        }

        TLX_HD
        qint8& operator+=(const qint8& other) {
            *this = *this + other;
            return *this;
        }
        TLX_HD
        qint8& operator-=(const qint8& other) {
            *this = *this - other;
            return *this;
        }
        TLX_HD
        qint8& operator*=(const qint8& other) {
            *this = *this * other;
            return *this;
        }
        TLX_HD
        qint8& operator/=(const qint8& other) {
            *this = *this / other;
            return *this;
        }

        TLX_HD
        bool operator==(const qint8& other) const {
            return this->m_value == other.m_value;
        }
        TLX_HD
        bool operator!=(const qint8& other) const {
            return this->m_value != other.m_value;
        }
        TLX_HD
        bool operator<(const qint8& other) const {
            return this->m_value < other.m_value;
        }
        TLX_HD
        bool operator>(const qint8& other) const {
            return this->m_value > other.m_value;
        }
        TLX_HD
        bool operator<=(const qint8& other) const {
            return this->m_value <= other.m_value;
        }
        TLX_HD
        bool operator>=(const qint8& other) const {
            return this->m_value >= other.m_value;
        }

        TLX_HD
        qint8& operator=(const qint8&) = default;
        TLX_HD
        qint8& operator=(qint8&&) noexcept = default;
    private:
        std::int8_t m_value;
    };
    std::ostream& operator<<(std::ostream& os, const qint8& value);

    struct alignas(2) qint16 {
        TLX_HD
        qint16() = default;
        TLX_HD
        explicit qint16(const std::int16_t value) {
            this->m_value = value;
        }
        TLX_HD
        qint16(const std::int32_t value) {
            this->m_value = static_cast<std::int16_t>(value);
        }
        TLX_HD
        qint16(const qint16&) = default;
        TLX_HD
        qint16(qint16&&) noexcept = default;

        explicit operator std::int32_t() const {
            return this->m_value;
        }

        TLX_HD
        qint16 operator+(const qint16& other) const {
            return static_cast<std::int32_t>(*this) + static_cast<std::int32_t>(other);
        }
        TLX_HD
        qint16 operator-(const qint16& other) const {
            return static_cast<std::int32_t>(*this) - static_cast<std::int32_t>(other);
        }
        TLX_HD
        qint16 operator*(const qint16& other) const {
            return static_cast<std::int32_t>(*this) * static_cast<std::int32_t>(other);
        }
        TLX_HD
        qint16 operator/(const qint16& other) const {
            return static_cast<std::int32_t>(*this) / static_cast<std::int32_t>(other);
        }

        TLX_HD
        qint16& operator+=(const qint16& other) {
            *this = *this + other;
            return *this;
        }
        TLX_HD
        qint16& operator-=(const qint16& other) {
            *this = *this - other;
            return *this;
        }
        TLX_HD
        qint16& operator*=(const qint16& other) {
            *this = *this * other;
            return *this;
        }
        TLX_HD
        qint16& operator/=(const qint16& other) {
            *this = *this / other;
            return *this;
        }

        TLX_HD
        bool operator==(const qint16& other) const {
            return this->m_value == other.m_value;
        }
        TLX_HD
        bool operator!=(const qint16& other) const {
            return this->m_value != other.m_value;
        }
        TLX_HD
        bool operator<(const qint16& other) const {
            return this->m_value < other.m_value;
        }
        TLX_HD
        bool operator>(const qint16& other) const {
            return this->m_value > other.m_value;
        }
        TLX_HD
        bool operator<=(const qint16& other) const {
            return this->m_value <= other.m_value;
        }
        TLX_HD
        bool operator>=(const qint16& other) const {
            return this->m_value >= other.m_value;
        }

        TLX_HD
        qint16& operator=(const qint16&) = default;
        TLX_HD
        qint16& operator=(qint16&&) noexcept = default;
    private:
        std::int16_t m_value;
    };
    std::ostream& operator<<(std::ostream& os, const qint16& value);
} //namespace tlx

#endif //TLX_QUANTIZE_TYPES_HPP