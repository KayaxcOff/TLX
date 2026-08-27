//
// Created by muham on 27.08.2026.
//

#ifndef TLX_LIST_HPP
#define TLX_LIST_HPP

#include <tlx/macros.hpp>
#include <initializer_list>

namespace tlx {
    template<typename T>
    class list {
    public:
        TLX_HD constexpr list() noexcept {
            this->m_first = nullptr;
            this->m_last = nullptr;
        }
        TLX_HD constexpr list(const T* first, const T* last) noexcept {
            this->m_first = first;
            this->m_last = last;
        }
        TLX_HD constexpr list(std::initializer_list<T> list) noexcept {
            this->m_first = list.begin();
            this->m_last = list.end();
        }
        TLX_HD ~list() = default;

        TLX_HD constexpr const T* data() const noexcept {
            return this->m_first;
        }
        TLX_HD constexpr const T* begin() const noexcept {
            return this->m_first;
        }
        TLX_HD constexpr const T* end() const noexcept {
            return this->m_last;
        }
        TLX_HD constexpr bool empty() const noexcept {
            return this->m_last == this->m_first;
        }
        constexpr std::size_t size() const noexcept {
            return empty() ? 0 : static_cast<std::size_t>(this->m_last - this->m_first);
        }
    private:
        const T* m_first;
        const T* m_last;
    };

    template<typename T, std::size_t N>
    TLX_HD constexpr list<T> make_list(const T (&array)[N]) noexcept {
        return list<T>(array, array + N);
    }
} //namespace tlx

#endif //TLX_LIST_HPP