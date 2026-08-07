//
// Created by muham on 27.07.2026.
//

#ifndef TLX_REFERENCE_HPP
#define TLX_REFERENCE_HPP

#include <tlx/macros.hpp>
#include <tlx/utility.hpp>

namespace tlx {
    /**
     * @brief A lightweight wrapper that stores a reference to an object of type T.
     *
     * This class allows references to be stored, copied and passed around
     * while preserving reference semantics. It is useful when you need to
     * treat a reference as a first-class object (e.g. in containers or
     * higher-order functions).
     *
     * @tparam T Type of the referenced object.
     */
    template<class T>
    class reference {
    public:
        TLX_HD constexpr reference() noexcept {
            this->m_value = nullptr;
        }
        /**
         * @brief Constructs a reference wrapper bound to the given object.
         *
         * @param value Object to bind to.
         */
        TLX_HD constexpr explicit reference(T& value) noexcept {
            this->m_value = ::tlx::addressOf(value);
        }
        TLX_HD reference(const reference&) = default;
        TLX_HD reference(reference&&) = default;
        TLX_HD constexpr explicit reference(T&&) = delete;
        TLX_HD ~reference() = default;

        TLX_HD constexpr operator T&() const noexcept {
            return *this->m_value;
        }
        TLX_HD constexpr explicit operator bool() const noexcept {
            return this->m_value != nullptr;
        }

        TLX_HD constexpr T* operator->() noexcept {
            return this->m_value;
        }
        TLX_HD constexpr const T* operator->() const noexcept {
            return this->m_value;
        }

        TLX_HD constexpr T& operator()() noexcept {
            return *this->m_value;
        }
        TLX_HD constexpr const T& operator()() const noexcept {
            return *this->m_value;
        }

        TLX_HD reference& operator=(const reference&) = default;
        TLX_HD reference& operator=(reference&&) = default;
    private:
        T* m_value;
    };

    /**
     * @brief Creates a reference wrapper for a non-const object.
     *
     * @tparam T Type of the object.
     * @param t Object to wrap.
     * @return reference<T> wrapper bound to t.
     */
    template<typename T>
    [[nodiscard]]
    TLX_HD constexpr reference<T> ref(T& t) {
        return reference<T>(t);
    }

    /**
     * @brief Creates a reference wrapper for a const object.
     *
     * @tparam T Type of the object.
     * @param t Const object to wrap.
     * @return reference<const T> Const reference wrapper bound to t.
     */
    template<typename T>
    [[nodiscard]]
    TLX_HD constexpr reference<const T> cref(const T& t) {
        return reference<const T>(t);
    }

    template<typename T>
    [[nodiscard]]
    TLX_HD reference<T> ref(T&&) = delete;
    template<typename T>
    [[nodiscard]]
    TLX_HD reference<const T> cref(const T&&) = delete;
} //namespace tlx

#endif //TLX_REFERENCE_HPP