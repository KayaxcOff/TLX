//
// Created by muham on 28.07.2026.
//

#ifndef TLX_STRING_HPP
#define TLX_STRING_HPP

#include <tlx/macros.hpp>
#include <ostream>
#include <string>
#include <string_view>

namespace tlx {
    constexpr std::size_t npos = static_cast<std::size_t>(-1);

    class vstring {
    public:
        TLX_HD vstring() {
            this->m_data = nullptr;
            this->m_size = 0;
        }
        TLX_HD vstring(const char* data, const std::size_t size) {
            this->m_data = data;
            this->m_size = size;
        }
        template<std::size_t N>
        TLX_HD vstring(const char (&data)[N]) {
            this->m_data = data;
            this->m_size = N - 1;
        }
        vstring(const std::string &data) {
            this->m_data = data.data();
            this->m_size = data.size();
        }
        TLX_HD vstring(const vstring&) = default;
        TLX_HD vstring(vstring&&) = default;

        [[nodiscard]]
        TLX_HD const char* data() const noexcept {
            return this->m_data;
        }
        [[nodiscard]]
        TLX_HD const char* c() const noexcept {
            return this->m_data;
        }
        [[nodiscard]]
        TLX_HD std::size_t size() const noexcept {
            return this->m_size;
        }
        [[nodiscard]]
        TLX_HD bool empty() const noexcept {
            return this->m_size == 0;
        }
        [[nodiscard]]
        TLX_HD const char* begin() const noexcept {
            return this->m_data;
        }
        [[nodiscard]]
        TLX_HD const char* end() const noexcept {
            return this->m_data + this->m_size;
        }
        [[nodiscard]]
        TLX_HD const char* cbegin() const noexcept {
            return this->m_data;
        }
        [[nodiscard]]
        TLX_HD const char* cend() const noexcept {
            return this->m_data + this->m_size;
        }
        [[nodiscard]]
        const char& front() const noexcept {
            return this->m_data[0];
        }
        [[nodiscard]]
        const char& back() const noexcept {
            return this->m_data[this->m_size - 1];
        }
        [[nodiscard]]
        TLX_HD bool start_with(const char& value) const {
            return this->m_data[0] == value;
        }
        [[nodiscard]]
        TLX_HD bool end_with(const char& value) const {
            return this->m_data[this->m_size - 1] == value;
        }
        [[nodiscard]]
        TLX_HD vstring substr(const std::size_t pos, const std::size_t count = npos) const noexcept {
            if (pos >= this->m_size) {
                return {};
            }

            const std::size_t len = (count > this->m_size - pos) ? (this->m_size - pos) : count;

            return { this->m_data + pos, len };
        }

        [[nodiscard]]
        std::string ToString() const {
            return {this->m_data, this->m_size};
        }

        operator std::string_view() const noexcept {
            return {this->m_data, this->m_size};
        }

        TLX_HD const char &operator[](const std::size_t index) const noexcept {
            return this->m_data[index];
        }
        TLX_HD bool operator==(const vstring &other) const noexcept {
            if (this->m_size != other.m_size) {
                return false;
            }
            for (std::size_t i = 0; i < this->m_size; ++i) {
                if (this->m_data[i] != other.m_data[i]) {
                    return false;
                }
            }
            return true;
        }
        TLX_HD bool operator!=(const vstring &other) const noexcept {
            return !((*this) == other);
        }
        TLX_HD vstring& operator=(const vstring&) = default;
        TLX_HD vstring& operator=(vstring&&) noexcept = default;
    private:
        const char* m_data;
        std::size_t m_size;
    };

    std::ostream& operator<<(std::ostream& os, const vstring& value);
} //namespace tlx

#endif //TLX_STRING_HPP