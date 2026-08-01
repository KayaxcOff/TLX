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
    template<typename T, std::size_t N>
    class vec {
    public:
        TLX_HD vec() {
            this->m_data = nullptr;
            this->m_size = 0;
        };
        TLX_HD vec(std::size_t size) {
            this->m_size = size;
        }
        TLX_HD vec(std::initializer_list<T> list) {
            if (list.size() > N) {
                throw Exception("Initializer list exceed vector capacity");
            }
            this->m_size = list.size();
            std::size_t i = 0;
            for (const auto& item : list) {
                ::tlx::construct(this->data() + i, item);
                i++;
            }
        }
        TLX_HD vec(const vec& other) {
            this->m_size = other.m_size;
            for (std::size_t i = 0; i < this->m_size; i++) {
                ::tlx::construct(this->data() + i, other[i]);
            }
        }
        TLX_HD vec(vec&& other) noexcept {
            this->m_size = other.m_size;
            for (std::size_t i = 0; i < this->m_size; i++) {
                ::tlx::construct(this->data() + i, ::tlx::move(other[i]));
                ::tlx::destroy(other.data() + i);
            }
            other.m_size = 0;
        }
        TLX_HD ~vec() {
            this->clear();
        }

        TLX_HD void clear() {
            for (std::size_t i = 0; i < this->m_size; i++) {
                ::tlx::destroy(data() + i);
            }
            this->m_size = 0;
        }

        [[nodiscard]]
        TLX_HD T* data() {
            return reinterpret_cast<T*>(this->m_data);
        }
        [[nodiscard]]
        TLX_HD const T* data() const noexcept {
            return reinterpret_cast<const T*>(this->m_data);
        }
        [[nodiscard]]
        TLX_HD std::size_t size() const noexcept {
            return this->m_size;
        }
        [[nodiscard]]
        TLX_HD static std::size_t capacity() noexcept {
            return N;
        }
        [[nodiscard]]
        bool empty() const noexcept {
            return this->m_size == 0;
        }

        [[nodiscard]]
        TLX_HD T* begin() noexcept {
            return data();
        }
        [[nodiscard]]
        TLX_HD const T* begin() const noexcept {
            return data();
        }
        [[nodiscard]]
        TLX_HD T* end() noexcept {
            return data() + this->m_size;
        }
        [[nodiscard]]
        TLX_HD const T* end() const noexcept {
            return data() + this->m_size;
        }
        [[nodiscard]]
        TLX_HD const T* cbegin() const noexcept {
            return data();
        }
        [[nodiscard]]
        TLX_HD const T* cend() const noexcept {
            return data() + this->m_size;
        }
        [[nodiscard]]
        TLX_HD T const& front() const noexcept {
            return this->m_data[0];
        }
        [[nodiscard]]
        TLX_HD T const& back() const noexcept {
            return this->m_data[this->m_size - 1];
        }

        TLX_HD void push_back(const T& value) {
            //TLX_EXIT_IF(this->m_size >= N, "Vector capacity exceeded");
            ::tlx::construct(this->data() + this->m_size, value);
            ++this->m_size;
        }

        TLX_HD void push_back(T&& value) {
            //TLX_EXIT_IF(this->m_size >= N, "Vector capacity exceeded");
            ::tlx::construct(this->data() + this->m_size, ::tlx::move(value));
            ++this->m_size;
        }

        template<typename... Args>
        TLX_HD T& emplace_back(Args&&... args) {
            //TLX_EXIT_IF(this->m_size >= N, "Vector capacity exceeded");
            ::tlx::construct(this->data() + this->m_size, ::tlx::forward<Args>(args)...);
            ++this->m_size;
            return this->back();
        }

        TLX_HD void pop_back() {
            //TLX_EXIT_IF(this->empty(), "pop_back on empty vector");
            --this->m_size;
            ::tlx::destroy(this->data() + this->m_size);
        }

        TLX_HD T* insert(std::size_t index, const T& value) {
            //TLX_EXIT_IF(index > this->m_size, "insert: index out of bounds");
            //TLX_EXIT_IF(this->m_size >= N, "Vector capacity exceeded");

            if (index == this->m_size) {
                ::tlx::construct(this->data() + index, value);
            } else {
                ::tlx::construct(this->data() + this->m_size, ::tlx::move(this->data()[this->m_size - 1]));
                for (std::size_t i = this->m_size - 1; i > index; --i) {
                    this->data()[i] = ::tlx::move(this->data()[i - 1]);
                }
                this->data()[index] = value;
            }
            ++this->m_size;
            return this->data() + index;
        }

        TLX_HD T* erase(std::size_t index) {
            //TLX_EXIT_IF(index >= this->m_size, "erase: index out of bounds");
            for (std::size_t i = index; i + 1 < this->m_size; i++) {
                this->data()[i] = ::tlx::move(this->data()[i + 1]);
            }
            --this->m_size;
            ::tlx::destroy(this->data() + this->m_size);
            return this->data() + index;
        }

        TLX_HD void resize(std::size_t new_size) {
            //TLX_EXIT_IF(new_size > N, "Vector capacity exceeded");
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
           // TLX_EXIT_IF(new_size > N, "Vector capacity exceeded");
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
            //TLX_EXIT_IF(list.size() > N, "assign: list exceeds vector capacity");
            this->clear();
            std::size_t i = 0;
            for (const auto& item : list) {
                ::tlx::construct(this->data() + i, item);
                i++;
            }
            this->m_size = list.size();
        }

        TLX_HD void assign(std::size_t count, const T& value) {
            //TLX_EXIT_IF(count > N, "assign: count exceeds vector capacity");
            this->clear();
            for (std::size_t i = 0; i < count; i++) {
                ::tlx::construct(this->data() + i, value);
            }
            this->m_size = count;
        }

        TLX_HOST operator std::vector<T>() noexcept {
            return {this->begin(), this->end()};
        }
        [[nodiscard]]
        TLX_HD T& operator[](const std::size_t index) {
            if (index >= this->m_size) {
                throw Exception("Out of bounds access");
            }
            return this->data()[index];
        }

        [[nodiscard]]
        TLX_HD const T& operator[](const std::size_t index) const {
            if (index >= this->m_size) {
                throw Exception("Out of bounds access");
            }
            return this->data()[index];
        }

        TLX_HD vec& operator=(const vec& other) {
            if (this != &other) {
                this->clear();
                this->m_size = other.m_size;
                for (std::size_t i = 0; i < this->m_size; i++) {
                    ::tlx::construct(this->data() + i, other[i]);
                }
            }
            return *this;
        }

        TLX_HD vec& operator=(vec&& other) noexcept {
            if (this != &other) {
                this->clear();
                this->m_size = other.m_size;
                for (std::size_t i = 0; i < this->m_size; i++) {
                    ::tlx::construct(this->data() + i, ::tlx::move(other[i]));
                    ::tlx::destroy(other.data() + i);
                }
                other.m_size = 0;
            }
            return *this;
        }
    private:
        alignas(T) std::byte m_data[N * sizeof(T)]{};
        std::size_t m_size;
    };
} //namespace tlx


#endif //TLX_VECTOR_HPP