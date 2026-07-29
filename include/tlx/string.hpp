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

    /**
     * @brief A non-owning view over a contiguous sequence of characters.
     *
     * `vstring` stores a pointer to the beginning of a character sequence
     * and its length. It does not own the memory and therefore does not
     * allocate or free it. It is intentionally lightweight and can be used
     * on both host and device code.
     */
    class vstring {
    public:
        /**
         * @brief Default constructor. Creates an empty view.
         */
        TLX_HD vstring() {
            this->m_data = nullptr;
            this->m_size = 0;
        }
        /**
         * @brief Constructs a view from a pointer and a size.
         *
         * @param data Pointer to the first character.
         * @param size Number of characters in the view.
         */
        TLX_HD vstring(const char* data, const std::size_t size) {
            this->m_data = data;
            this->m_size = size;
        }
        /**
         * @brief Constructs a view from a string literal (array).
         *
         * The size is automatically deduced as `N - 1` (excluding the null terminator).
         *
         * @tparam N Size of the character array (including null terminator).
         * @param data Character array.
         */
        template<std::size_t N>
        TLX_HD vstring(const char (&data)[N]) {
            this->m_data = data;
            this->m_size = N - 1;
        }
        /**
         * @brief Constructs a view from a `std::string`.
         *
         * @param data Source string.
         */
        vstring(const std::string &data) {
            this->m_data = data.data();
            this->m_size = data.size();
        }
        TLX_HD vstring(const vstring&) = default;
        TLX_HD vstring(vstring&&) = default;

        /**
         * @brief Returns a pointer to the underlying character data.
         *
         * @return const char* Pointer to the first character.
         */
        [[nodiscard]]
        TLX_HD const char* data() const noexcept {
            return this->m_data;
        }
        /**
         * @brief Alias for `data()`. Returns a C-style string pointer.
         *
         * @return const char* Pointer to the first character.
         */
        [[nodiscard]]
        TLX_HD const char* c() const noexcept {
            return this->m_data;
        }
        /**
         * @brief Returns the number of characters in the view.
         *
         * @return std::size_t Length of the string view.
         */
        [[nodiscard]]
        TLX_HD std::size_t size() const noexcept {
            return this->m_size;
        }
        /**
         * @brief Checks whether the view is empty.
         *
         * @return true if the size is zero.
         */
        [[nodiscard]]
        TLX_HD bool empty() const noexcept {
            return this->m_size == 0;
        }
        /**
         * @brief Returns an iterator to the beginning of the view.
         *
         * @return const char* Pointer to the first character.
         */
        [[nodiscard]]
        TLX_HD const char* begin() const noexcept {
            return this->m_data;
        }
        /**
         * @brief Returns an iterator to the end of the view.
         *
         * @return const char* Pointer one past the last character.
         */
        [[nodiscard]]
        TLX_HD const char* end() const noexcept {
            return this->m_data + this->m_size;
        }
        /**
         * @brief Returns a const iterator to the beginning of the view.
         *
         * @return const char* Pointer to the first character.
         */
        [[nodiscard]]
        TLX_HD const char* cbegin() const noexcept {
            return this->m_data;
        }
        /**
         * @brief Returns a const iterator to the end of the view.
         *
         * @return const char* Pointer one past the last character.
         */
        [[nodiscard]]
        TLX_HD const char* cend() const noexcept {
            return this->m_data + this->m_size;
        }
        /**
         * @brief Returns a reference to the first character.
         *
         * @return const char& Reference to the first character.
         * @note Undefined behavior if the view is empty.
         */
        [[nodiscard]]
        const char& front() const noexcept {
            return this->m_data[0];
        }
        /**
         * @brief Returns a reference to the last character.
         *
         * @return const char& Reference to the last character.
         * @note Undefined behavior if the view is empty.
         */
        [[nodiscard]]
        const char& back() const noexcept {
            return this->m_data[this->m_size - 1];
        }
        /**
         * @brief Checks whether the view starts with the given character.
         *
         * @param value Character to compare with the first character.
         * @return true if the first character equals `value`.
         * @note Undefined behavior if the view is empty.
         */
        [[nodiscard]]
        TLX_HD bool start_with(const char& value) const {
            return this->m_data[0] == value;
        }
        /**
         * @brief Checks whether the view ends with the given character.
         *
         * @param value Character to compare with the last character.
         * @return true if the last character equals `value`.
         * @note Undefined behavior if the view is empty.
         */
        [[nodiscard]]
        TLX_HD bool end_with(const char& value) const {
            return this->m_data[this->m_size - 1] == value;
        }
        /**
         * @brief Returns a substring view.
         *
         * @param pos Starting position (zero-based).
         * @param count Maximum number of characters to include (default: until the end).
         * @return vstring A new view representing the requested substring.
         *         Returns an empty view if `pos` is out of range.
         */
        [[nodiscard]]
        TLX_HD vstring substr(const std::size_t pos, const std::size_t count = npos) const noexcept {
            if (pos >= this->m_size) {
                return {};
            }

            const std::size_t len = (count > this->m_size - pos) ? (this->m_size - pos) : count;

            return { this->m_data + pos, len };
        }

        /**
         * @brief Creates an owning `std::string` from this view.
         *
         * @return std::string A new string containing a copy of the viewed characters.
         */
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