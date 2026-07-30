//
// Created by muham on 30.07.2026.
//

#ifndef TLX_SPAN_HPP
#define TLX_SPAN_HPP

#include <tlx/buffer.hpp>
#include <tlx/utility.hpp>

namespace tlx {
    /**
     * @brief A non-owning view over a contiguous sequence of elements of type T.
     *
     * `Span` stores a view of a contiguous memory region interpreted as
     * an array of `T`. It does not own the underlying memory and therefore
     * does not allocate or free it. The class is intentionally lightweight
     * and provides basic random-access and iterator support.
     *
     * @tparam T Type of the elements in the span.
     */
    template<typename T>
    class Span {
    public:
        /**
         * @brief Default constructor. Creates an empty span.
         */
        Span() = default;
        /**
         * @brief Constructs a span from a pointer and a size (in elements).
         *
         * @param data Pointer to the first element.
         * @param size Number of elements in the span.
         */
        Span(T* data, const std::size_t size) : m_buffer(reinterpret_cast<std::byte*>(data), size * sizeof(T)) {}
        /**
         * @brief Constructs a span from a C-style array.
         *
         * @tparam N Size of the array.
         * @param arr Reference to the array.
         */
        template<std::size_t N>
        explicit Span(const T (&arr)[N]) : m_buffer(reinterpret_cast<std::byte*>(arr), N * sizeof(T)) {}
        /**
         * @brief Constructs a span from a pair of iterators (pointers).
         *
         * @param first Pointer to the first element.
         * @param last  Pointer one past the last element.
         */
        Span(const T* first, const T* last) : m_buffer(reinterpret_cast<std::byte*>(first), (last - first) * sizeof(T)) {}
        Span(const Span&) = default;
        Span(Span&&) noexcept = default;
        ~Span() = default;

        /**
         * @brief Returns a pointer to the first element.
         *
         * @return T* Pointer to the beginning of the span.
         */
        [[nodiscard]]
        T* data() {
            return this->m_buffer.as<T>();
        }
        /**
         * @brief Returns a const pointer to the first element.
         *
         * @return const T* Const pointer to the beginning of the span.
         */
        [[nodiscard]]
        const T* data() const noexcept {
            return this->m_buffer.as<T>();
        }
        /**
         * @brief Returns the number of elements in the span.
         *
         * @return std::size_t Number of elements.
         */
        [[nodiscard]]
        std::size_t size() const noexcept {
            return this->m_buffer.size() / sizeof(T);
        }
        /**
         * @brief Checks whether the span is empty.
         *
         * @return true if the span contains no elements.
         */
        [[nodiscard]]
        bool empty() const noexcept {
            return this->m_buffer.empty();
        }
        /**
         * @brief Returns an iterator to the beginning of the span.
         *
         * @return T* Pointer to the first element.
         */
        [[nodiscard]]
        T* begin() noexcept {
            return this->data();
        }
        /**
         * @brief Returns a const iterator to the beginning of the span.
         *
         * @return const T* Const pointer to the first element.
         */
        [[nodiscard]]
        const T* begin() const noexcept {
            return this->data();
        }
        /**
         * @brief Returns an iterator to the end of the span.
         *
         * @return T* Pointer one past the last element.
         */
        [[nodiscard]]
        T* end() noexcept {
            return this->data() + this->size();
        }
        /**
         * @brief Returns a const iterator to the end of the span.
         *
         * @return const T* Const pointer one past the last element.
         */
        [[nodiscard]]
        const T* end() const noexcept {
            return this->data() + this->size();
        }

        T& operator[](std::size_t index) {
            return this->data()[index];
        }
        const T& operator[](std::size_t index) const noexcept {
            return this->data()[index];
        }

        Span& operator=(const Span&) = default;
        Span& operator=(Span&&) noexcept = default;
    private:
        BufferView m_buffer;
    };
} //namespace tlx

#endif //TLX_SPAN_HPP