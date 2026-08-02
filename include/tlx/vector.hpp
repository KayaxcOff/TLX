//
// Created by muham on 1.08.2026.
//

#ifndef TLX_VECTOR_HPP
#define TLX_VECTOR_HPP

#include <tlx/exception.hpp>
#include <tlx/macros.hpp>
#include <tlx/memory.hpp>
#include <tlx/utility.hpp>
#include <initializer_list>
#include <vector>

namespace tlx {
    template <typename T, std::size_t N>
    class vec {
    public:
        TLX_HD vec() : m_data{} {
            this->m_size = 0;
        }
        TLX_HD explicit vec(std::size_t size) : m_data{} {
            if (size > N) {
                throw Exception("vector size too large");
            }
            this->m_size = size;
        }
        TLX_HD vec(const std::initializer_list<T> &list) : m_data{} {
            if (list.size() > N) {
                throw Exception("Initializer list exceed vector capacity");
            }
            this->m_size = list.size();
            std::size_t i = 0;
            for (const auto& item : list) {
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
        }
        TLX_HD ~vec() {
            clear();
        }

        [[nodiscard]]
        TLX_HD static std::size_t capacity() noexcept {
            return N;
        }

        [[nodiscard]]
        TLX_HD T* data() {
            return reinterpret_cast<T*>(this->m_data);
        }
        [[nodiscard]]
        TLX_HD const T* data() const {
            return reinterpret_cast<const T*>(this->m_data);
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
        TLX_HD T* begin() noexcept {
            return data();
        }
        [[nodiscard]]
        TLX_HD T* end() noexcept {
            return data() + this->size();
        }
        [[nodiscard]]
        const T* begin() const noexcept {
            return data();
        }
        [[nodiscard]]
        const T* end() const noexcept {
            return data() + this->size();
        }
        [[nodiscard]]
        const T* cbegin() const noexcept {
            return data();
        }
        [[nodiscard]]
        const T* cend() const noexcept {
            return data() + this->size();
        }
        [[nodiscard]]
        const T& front() const noexcept {
            return data()[0];
        }
        [[nodiscard]]
        const T& back() const noexcept {
            return data()[this->m_size - 1];
        }

        TLX_HD void push(const T& item) noexcept {
            ::tlx::construct(data() + this->m_size, item);
            ++this->m_size;
        }
        TLX_HD void push(T&& item) noexcept {
            ::tlx::construct(data() + this->m_size, ::tlx::move(item));
            ++this->m_size;
        }
        template<typename ... Args>
        TLX_HD void emplace(Args&&... args) {
            ::tlx::construct(data() + this->m_size, ::tlx::forward<Args>(args)...);
            ++this->m_size;
        }
        TLX_HD void pop() noexcept {
            --this->m_size;
            ::tlx::destroy(data() + this->m_size);
        }
        TLX_HD void resize(std::size_t new_size) noexcept {
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
        TLX_HD void resize(std::size_t new_size, const T& value) {
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
        TLX_HD void assign(std::initializer_list<T> list) {
            this->clear();
            std::size_t i = 0;
            for (const auto& item : list) {
                ::tlx::construct(this->data() + i, item);
                i++;
            }
            this->m_size = list.size();
        }

        TLX_HD void assign(std::size_t count, const T& value) {
            this->clear();
            for (std::size_t i = 0; i < count; i++) {
                ::tlx::construct(this->data() + i, value);
            }
            this->m_size = count;
        }
        TLX_HD void clear() {
            for (std::size_t i = 0; i < this->m_size; i++) {
                ::tlx::destroy(this->data() + i);
            }
            this->m_size = 0;
        }

        [[nodiscard]]
        TLX_HD T* insert(std::size_t index, const T& item) noexcept {
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
        [[nodiscard]]
        TLX_HD T* erase(std::size_t index) noexcept {
            for (std::size_t i = index; i + 1 < this->m_size; i++) {
                data()[i] = ::tlx::move(data()[i + 1]);
            }
            --this->m_size;
            ::tlx::destroy(data() + this->m_size);
            return data() + index;
        }

        TLX_HD explicit operator std::vector<T>() noexcept {
            return {begin(), end()};
        }

        [[nodiscard]]
        TLX_HD T& operator[](std::size_t index) noexcept {
            if (index >= this->m_size) {
                throw Exception("Index out of bounds");
            }
            return data()[index];
        }
        [[nodiscard]]
        TLX_HD const T& operator[](std::size_t index) const noexcept {
            if (index >= this->m_size) {
                throw Exception("Index out of bounds");
            }
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
                    ::tlx::destroy(other[i]);
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