//
// Created by muham on 25.07.2026.
//

#ifndef TLX_MEMORY_HPP
#define TLX_MEMORY_HPP

#include <tlx/concepts.hpp>
#include <tlx/utility.hpp>
#include <cstddef>

namespace tlx {
    /**
     * @brief Checks whether two pointers point to the same address.
     *
     * @tparam Ty Type of the pointed-to objects.
     * @param t1 First pointer.
     * @param t2 Second pointer.
     * @return true if both pointers are equal.
     */
    template<class Ty>
    [[nodiscard]]
    constexpr bool isSameAddress(const Ty* t1, const Ty* t2) {
        return t1 == t2;
    }
    /**
     * @brief Checks whether two objects occupy the same address.
     *
     * @tparam Ty Type of the objects.
     * @param t1 First object.
     * @param t2 Second object.
     * @return true if both objects have the same address.
     */
    template<class Ty>
    [[nodiscard]]
    constexpr bool isSameAddress(const Ty& t1, const Ty& t2) {
        return ::tlx::addressOf(t1) == ::tlx::addressOf(t2);
    }

    /**
     * @brief Checks whether a pointer is null.
     *
     * @tparam Ty Type of the pointed-to object.
     * @param t Pointer to check.
     * @return true if the pointer is nullptr.
     */
    template<class Ty>
    [[nodiscard]]
    constexpr bool isNull(const Ty* t) {
        return t == nullptr;
    }

    /**
     * @brief Checks whether a pointer is aligned to the given boundary.
     *
     * @tparam Ty Type of the pointed-to object.
     * @param t Pointer to check.
     * @param alignment Desired alignment in bytes.
     * @return true if the pointer is properly aligned.
     */
    template<class Ty>
    [[nodiscard]]
    constexpr bool isAligned(const Ty* t, const std::size_t alignment) {
        return (reinterpret_cast<std::uintptr_t>(t) % alignment) == 0;
    }

    /**
     * @brief Checks whether an address is aligned to the given boundary.
     *
     * @param address Memory address to check.
     * @param alignment Desired alignment in bytes.
     * @return true if the address is properly aligned.
     */
    [[nodiscard]]
    bool isAligned(std::uintptr_t address, std::size_t alignment) noexcept;
    /**
     * @brief Aligns a value upward to the next multiple of the given alignment.
     *
     * @param value Value to align.
     * @param alignment Alignment boundary (must be a power of two).
     * @return std::uintptr_t The aligned value.
     */
    [[nodiscard]]
    std::uintptr_t alignUp(std::uintptr_t value, std::size_t alignment) noexcept;
    /**
     * @brief Aligns a value downward to the previous multiple of the given alignment.
     *
     * @param value Value to align.
     * @param alignment Alignment boundary (must be a power of two).
     * @return std::uintptr_t The aligned value.
     */
    [[nodiscard]]
    std::uintptr_t alignDown(std::uintptr_t value, std::size_t alignment);

    /**
     * @brief Computes the distance (in elements) between two pointers.
     *
     * @tparam Ty Type of the pointed-to objects.
     * @param first Pointer to the first element.
     * @param last Pointer to the last element.
     * @return std::ptrdiff_t Number of elements between the two pointers.
     */
    template<class Ty>
    [[nodiscard]]
    constexpr std::ptrdiff_t distance(const Ty* first, const Ty* last) noexcept {
        return last - first;
    }

    /**
     * @brief Returns a pointer offset by a given number of bytes.
     *
     * @tparam Ty Type of the pointed-to object.
     * @param ptr Original pointer.
     * @param offset Number of bytes to offset.
     * @return Ty* Pointer advanced by the specified number of bytes.
     */
    template<class Ty>
    [[nodiscard]]
    Ty* offset(Ty* ptr, const std::size_t offset) {
        return reinterpret_cast<Ty*>(reinterpret_cast<std::byte*>(ptr) + offset);
    }

    /**
     * @brief Allocates aligned memory.
     *
     * @param bytes Number of bytes to allocate.
     * @param alignment Desired alignment in bytes.
     * @return void* Pointer to the allocated memory, or nullptr on failure.
     */
    [[nodiscard]]
    void* malloc(std::size_t bytes, std::size_t alignment);
    /**
     * @brief Frees memory previously allocated with tlx::malloc.
     *
     * @param ptr Pointer previously returned by tlx::malloc.
     * @param alignment Alignment used during allocation.
     */
    void free(void* ptr, std::size_t alignment) noexcept;

    /**
     * @brief Constructs an object in-place at the given memory location.
     *
     * @tparam Ty Type of the object to construct.
     * @tparam Args Constructor argument types.
     * @param ptr Pointer to the uninitialized memory.
     * @param args Constructor arguments.
     * @return Ty* Pointer to the newly constructed object.
     */
    template<class Ty, class... Args>
    constexpr Ty* construct(Ty* ptr, Args&&... args) {
        return ::new(static_cast<void*>(ptr)) Ty(static_cast<Args&&>(args)...);
    }
    /**
     * @brief Destroys an object without deallocating its memory.
     *
     * @tparam Ty Type of the object to destroy.
     * @param ptr Pointer to the object.
     */
    template<class Ty>
    constexpr void destroy(Ty* ptr) noexcept {
        ptr->~Ty();
    }

    /**
     * @brief A simple owning smart pointer.
     *
     * This class manages the lifetime of a dynamically allocated object of type `T`.
     * It provides basic ownership semantics, construction from arguments, and
     * automatic destruction when the pointer goes out of scope.
     *
     * @tparam T Type of the managed object.
     */
    template<typename T>
    class ptr {
    public:
        /**
         * @brief Default constructor. Creates a null pointer.
         */
        ptr() noexcept {
            this->m_value = nullptr;
        }
        /**
         * @brief Constructs from a raw pointer (takes ownership).
         *
         * @param ptr Raw pointer to manage.
         */
        explicit ptr(T* ptr) noexcept {
            this->m_value = ptr;
        }
        /**
         * @brief Constructs a new object of type T in-place and takes ownership.
         *
         * @tparam Args Constructor argument types.
         * @param args Arguments forwarded to the constructor of T.
         */
        template<typename... Args> requires ::tlx::constructible_from<T, Args...>
        explicit ptr(Args&&... args) {
            this->m_value = nullptr;
            this->m_value = static_cast<T *>(::operator new(sizeof(T)));
            ::tlx::construct(this->m_value, ::tlx::forward<Args>(args)...);
        }
        ptr(const ptr&) = default;
        ptr(ptr&&) noexcept = default;
        ~ptr() {
            if (this->m_value != nullptr) {
                ::tlx::destroy(this->m_value);
                ::operator delete(this->m_value);
            }
        }

        /**
         * @brief Creates a ptr from a raw value reinterpreted as a pointer.
         *
         * @param t Value to reinterpret as a pointer.
         * @return ptr Newly created pointer.
         */
        [[nodiscard]]
        static ptr cast(T t) {
            return ptr(reinterpret_cast<T*>(t));
        }

        /**
         * @brief Returns the managed raw pointer.
         *
         * @return T* Pointer to the managed object.
         */
        [[nodiscard]]
        T* get() {
            return this->m_value;
        }
        /**
         * @brief Returns the managed raw pointer (const version).
         *
         * @return const T* Pointer to the managed object.
         */
        [[nodiscard]]
        const T* get() const {
            return this->m_value;
        }
        /**
         * @brief Releases ownership and destroys the managed object.
         */
        void reset() {
            ::tlx::destroy(this->m_value);
            ::operator delete(this->m_value);
            this->m_value = nullptr;
        }

        T* operator->() noexcept {
            return this->m_value;
        }
        T& operator*() noexcept {
            return *this->m_value;
        }
        explicit operator bool() const {
            return this->m_value != nullptr;
        }

        ptr& operator=(const ptr& other) = default;
        ptr& operator=(ptr&& other) noexcept = default;
    private:
        T* m_value;
    };
} //namespace tlx

#endif //TLX_MEMORY_HPP