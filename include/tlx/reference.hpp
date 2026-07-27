//
// Created by muham on 27.07.2026.
//

#ifndef TLX_REFERENCE_HPP
#define TLX_REFERENCE_HPP

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
        reference() = delete;
        /**
         * @brief Constructs a reference wrapper bound to the given object.
         *
         * @param value Object to bind to.
         */
        explicit reference(T& value) {
            this->m_value = ::tlx::addressOf(value);
        }
        reference(const reference&) = default;
        reference(reference&&) = default;
        ~reference() = default;

        explicit operator T&() const noexcept {
            return *this->m_value;
        }
        explicit operator bool() const noexcept {
            return this->m_value;
        }

        T* operator->() noexcept {
            return this->m_value;
        }
        const T* operator->() const noexcept {
            return this->m_value;
        }

        T& operator()() noexcept {
            return *this->m_value;
        }
        const T& operator()() const noexcept {
            return *this->m_value;
        }

        reference& operator=(const reference&) = default;
        reference& operator=(reference&&) = default;
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
    reference<T> ref(T& t) {
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
    reference<const T> cref(const T& t) {
        return reference<const T>(t);
    }
} //namespace tlx

#endif //TLX_REFERENCE_HPP