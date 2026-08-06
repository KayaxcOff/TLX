//
// Created by muham on 1.08.2026.
//

#ifndef TLX_VECTOR_HPP
#define TLX_VECTOR_HPP

#include <tlx/errors.hpp>
#include <tlx/macros.hpp>
#include <tlx/memory.hpp>
#include <tlx/utility.hpp>
#include <initializer_list>
#include <vector>

namespace tlx {
    /**
     * @brief Fixed-capacity contiguous sequence container.
     *
     * `vec<T, N>` is a stack-allocated vector with a maximum capacity of `N`
     * elements. It provides a subset of the interface of `std::vector` while
     * remaining usable in both host and device code. Elements are constructed
     * and destroyed manually.
     *
     * @tparam T Type of the elements.
     * @tparam N Maximum number of elements the vector can hold.
     */
    template <typename T, std::size_t N>
    class vec {
    public:
        /**
         * @brief Default constructor. Creates an empty vector.
         */
        TLX_HD vec() : m_data{} {
            this->m_size = 0;
        }
        /**
         * @brief Constructs a vector with the given size (elements are default-constructed).
         *
         * @param size Number of elements (must not exceed capacity).
         */
        TLX_HD explicit vec(std::size_t size) : m_data{} {
            TLX_HD_ERROR(size > N, "Vector size too large");
            this->m_size = size;
        }
        /**
         * @brief Constructs a vector from an initializer list.
         *
         * @param list Initializer list of elements.
         */
        TLX_HD vec(const std::initializer_list<T> &list) : m_data{} {
            TLX_HD_ERROR(list.size() > N, "Vector size too large");
            this->m_size = list.size();
            std::size_t i = 0;
            for (const auto& item : list) {
                ::tlx::construct(data() + i, item);
                i++;
            }
        }
        explicit vec(const std::vector<T>& vector) : m_data{} {
            TLX_HD_ERROR(vector.size() > N, "Vector size too large");
            this->m_size = vector.size();
            std::size_t i = 0;
            for (const auto& item : vector) {
                ::tlx::construct(data() + i, item);
                i++;
            }
        }
        TLX_HD vec(const vec& other) : m_data{} {
            this->m_size = other.m_size;
            for (std::size_t i = 0; i < this->m_size; i++) {
                ::tlx::construct(data() + i, other[i]);
            }
        }
        TLX_HD vec(vec&& other) noexcept : m_data{} {
            this->m_size = other.m_size;
            for (std::size_t i = 0; i < this->m_size; i++) {
                ::tlx::construct(data() + i, ::tlx::move(other[i]));
                ::tlx::destroy(other.data() + i);
            }
            other.m_size = 0;
        }
        TLX_HD ~vec() {
            clear();
        }

        /**
         * @brief Returns the maximum number of elements the vector can hold.
         *
         * @return std::size_t Capacity (equal to the template parameter N).
         */
        [[nodiscard]]
        TLX_HD static std::size_t capacity() noexcept {
            return N;
        }

        /**
         * @brief Returns a pointer to the underlying array.
         *
         * @return T* Pointer to the first element.
         */
        [[nodiscard]]
        TLX_HD T* data() {
            return reinterpret_cast<T*>(this->m_data);
        }
        /**
         * @brief Returns a const pointer to the underlying array.
         *
         * @return const T* Const pointer to the first element.
         */
        [[nodiscard]]
        TLX_HD const T* data() const {
            return reinterpret_cast<const T*>(this->m_data);
        }
        /**
         * @brief Returns the number of elements currently stored.
         *
         * @return std::size_t Current size.
         */
        [[nodiscard]]
        TLX_HD std::size_t size() const noexcept {
            return this->m_size;
        }
        /**
         * @brief Checks whether the vector is empty.
         *
         * @return true if the size is zero.
         */
        [[nodiscard]]
        TLX_HD bool empty() const noexcept {
            return this->m_size == 0;
        }
        [[nodiscard]]
        TLX_HD T* begin() noexcept {
            return data();
        }
        [[nodiscard]]
        TLX_HD T* end() noexcept {
            return data() + this->size();
        }
        [[nodiscard]]
        TLX_HD const T* begin() const noexcept {
            return data();
        }
        [[nodiscard]]
        TLX_HD const T* end() const noexcept {
            return data() + this->size();
        }
        [[nodiscard]]
        TLX_HD const T* cbegin() const noexcept {
            return data();
        }
        [[nodiscard]]
        TLX_HD const T* cend() const noexcept {
            return data() + this->size();
        }
        /**
         * @brief Returns a reference to the first element.
         *
         * @note Undefined behavior if the vector is empty.
         */
        [[nodiscard]]
        TLX_HD const T& first() const noexcept {
            return data()[0];
        }
        /**
         * @brief Returns a reference to the last element.
         *
         * @note Undefined behavior if the vector is empty.
         */
        [[nodiscard]]
        TLX_HD const T& last() const noexcept {
            return data()[this->m_size - 1];
        }

        /**
         * @brief Appends an element by copy.
         *
         * @param item Element to append.
         */
        TLX_HD void push(const T& item) noexcept {
            TLX_HD_ERROR(this->m_size >= N, "Size is higher than capacity");
            ::tlx::construct(data() + this->m_size, item);
            ++this->m_size;
        }
        /**
         * @brief Appends an element by move.
         *
         * @param item Element to append.
         */
        TLX_HD void push(T&& item) noexcept {
            TLX_HD_ERROR(this->m_size >= N, "Size is higher than capacity");
            ::tlx::construct(data() + this->m_size, ::tlx::move(item));
            ++this->m_size;
        }
        /**
         * @brief Constructs an element in-place at the end.
         *
         * @tparam Args Constructor argument types.
         * @param args Arguments forwarded to the constructor of T.
         */
        template<typename ... Args>
        TLX_HD void emplace(Args&&... args) {
            TLX_HD_ERROR(this->m_size >= N, "Size is higher than capacity");
            ::tlx::construct(data() + this->m_size, ::tlx::forward<Args>(args)...);
            ++this->m_size;
        }
        /**
         * @brief Removes the last element.
         *
         * @note Undefined behavior if the vector is empty.
         */
        TLX_HD void pop() noexcept {
            --this->m_size;
            ::tlx::destroy(data() + this->m_size);
        }
        /**
         * @brief Resizes the vector (default-constructs new elements).
         *
         * @param new_size New size (must not exceed capacity).
         */
        TLX_HD void resize(std::size_t new_size) noexcept {
            TLX_HD_ERROR(new_size > N, "New size is higher than capacity");
            if (new_size < this->m_size) {
                for (std::size_t i = new_size; i < this->m_size; i++) {
                    ::tlx::destroy(this->data() + i);
                }
            } else {
                for (std::size_t i = this->m_size; i < new_size; ++i) {
                    ::tlx::construct(this->data() + i);
                }
            }
            this->m_size = new_size;
        }
        /**
         * @brief Resizes the vector, initializing new elements with a value.
         *
         * @param new_size New size (must not exceed capacity).
         * @param value    Value used to initialize new elements.
         */
        TLX_HD void resize(std::size_t new_size, const T& value) {
            TLX_HD_ERROR(new_size > N, "New size is higher than capacity");
            if (new_size < this->m_size) {
                for (std::size_t i = new_size; i < this->m_size; i++) {
                    ::tlx::destroy(this->data() + i);
                }
            } else {
                for (std::size_t i = this->m_size; i < new_size; ++i) {
                    ::tlx::construct(this->data() + i, value);
                }
            }
            this->m_size = new_size;
        }
        /**
         * @brief Replaces the contents with those from an initializer list.
         *
         * @param list Initializer list of new elements.
         */
        TLX_HD void assign(std::initializer_list<T> list) {
            TLX_HD_ERROR(list.size() > N, "New size is higher than capacity");
            this->clear();
            std::size_t i = 0;
            for (const auto& item : list) {
                ::tlx::construct(this->data() + i, item);
                i++;
            }
            this->m_size = list.size();
        }

        /**
         * @brief Replaces the contents with `count` copies of `value`.
         *
         * @param count Number of elements.
         * @param value Value to assign.
         */
        TLX_HD void assign(std::size_t count, const T& value) {
            TLX_HD_ERROR(count > N, "New size is higher than capacity");
            this->clear();
            for (std::size_t i = 0; i < count; i++) {
                ::tlx::construct(this->data() + i, value);
            }
            this->m_size = count;
        }
        /**
         * @brief Removes all elements.
         */
        TLX_HD void clear() {
            for (std::size_t i = 0; i < this->m_size; i++) {
                ::tlx::destroy(this->data() + i);
            }
            this->m_size = 0;
        }

        /**
         * @brief Inserts an element at the given index.
         *
         * @param index Position at which to insert.
         * @param item  Element to insert.
         * @return T*   Pointer to the inserted element.
         */
        [[nodiscard]]
        TLX_HD T* insert(std::size_t index, const T& item) noexcept {
            TLX_HD_ERROR(this->m_size >= N, "New size is higher than capacity");
            if (index == this->m_size) {
                ::tlx::construct(data() + index, item);
            } else {
                ::tlx::construct(data() + this->m_size, ::tlx::move(data()[this->m_size - 1]));
                for (std::size_t i = this->m_size - 1; i > index; --i) {
                    data()[i] = ::tlx::move(data()[i - 1]);
                }
                data()[index] = item;
            }
            ++this->m_size;
            return data() + index;
        }
        /**
         * @brief Erases the element at the given index.
         *
         * @param index Position of the element to erase.
         * @return T*   Pointer to the element that now occupies the erased position
         *              (or end() if the last element was erased).
         */
        [[nodiscard]]
        TLX_HD T* erase(std::size_t index) noexcept {
            for (std::size_t i = index; i + 1 < this->m_size; i++) {
                data()[i] = ::tlx::move(data()[i + 1]);
            }
            --this->m_size;
            ::tlx::destroy(data() + this->m_size);
            return data() + index;
        }

        TLX_HOST explicit operator std::vector<T>() noexcept {
            return {begin(), end()};
        }

        [[nodiscard]]
        TLX_HD T& operator[](std::size_t index) noexcept {
            TLX_HD_ERROR(index >= this->m_size, "Index out of bounds");
            return data()[index];
        }
        [[nodiscard]]
        TLX_HD const T& operator[](std::size_t index) const noexcept {
            TLX_HD_ERROR(index >= this->m_size, "Index out of bounds");
            return data()[index];
        }

        TLX_HD vec& operator=(const vec& other) {
            if (this != &other) {
                clear();
                this->m_size = other.m_size;
                for (std::size_t i = 0; i < this->m_size; i++) {
                    ::tlx::construct(data() + i, other[i]);
                }
            }
            return *this;
        }
        TLX_HD vec& operator=(vec&& other) noexcept {
            if (this != &other) {
                clear();
                this->m_size = other.m_size;
                for (std::size_t i = 0; i < this->m_size; i++) {
                    ::tlx::construct(data() + i, ::tlx::move(other[i]));
                    ::tlx::destroy(other.data() + i);
                }
                other.m_size = 0;
            }
            return *this;
        }
    private:
        alignas(T) std::byte m_data[N * sizeof(T)];
        std::size_t m_size;
    };
} //namespace tlx

#endif //TLX_VECTOR_HPP