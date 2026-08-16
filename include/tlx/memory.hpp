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
     * @brief A lightweight RAII wrapper around a raw pointer.
     *
     * This class provides object-oriented access to a dynamically allocated
     * object while exposing raw pointer semantics. It automatically constructs
     * and destroys the managed object, but it does not enforce ownership rules
     * beyond object lifetime management. Copying and assignment preserve raw
     * pointer behavior, leaving ownership semantics to the user.
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

    /**
     * @brief Creates a smart pointer from a raw pointer.
     *
     * Takes ownership of the specified raw pointer.
     *
     * @tparam T Type of the managed object.
     * @param t Raw pointer to manage.
     * @return ptr<T> Smart pointer owning the given object.
     */
    template<typename T>
    [[nodiscard]]
    constexpr ptr<T> make_ptr(T* t) {
        return ptr<T>(t);
    }

    /**
     * @brief Returns a pointer advanced by the specified number of elements.
     *
     * @tparam Ty Type of the pointed-to object.
     * @param ptr Pointer to advance.
     * @param n Number of elements to advance.
     * @return Ty* Pointer advanced by n elements.
     */
    template<class Ty>
    [[nodiscard]]
    constexpr Ty* next(Ty* ptr, std::ptrdiff_t n = 1) noexcept {
        return ptr + n;
    }
    /**
     * @brief Returns a pointer moved backward by the specified number of elements.
     *
     * @tparam Ty Type of the pointed-to object.
     * @param ptr Pointer to move backward.
     * @param n Number of elements to move backward.
     * @return Ty* Pointer moved backward by n elements.
     */
    template<class Ty>
    [[nodiscard]]
    constexpr Ty* prev(Ty* ptr, std::ptrdiff_t n = 1) noexcept {
        return ptr - n;
    }

    /**
     * @brief Copies a range of elements to another location.
     *
     * Copies the elements in the range [first, last) into the destination
     * beginning at dest. The source and destination ranges must not overlap.
     *
     * @tparam Ty Type of the elements.
     * @param first Pointer to the first source element.
     * @param last Pointer one past the last source element.
     * @param dest Pointer to the destination range.
     * @return Ty* Pointer one past the last copied element.
     */
    template<class Ty>
    constexpr Ty* copy(const Ty* first, const Ty* last, Ty* dest) {
        while (first != last) {
            *dest++ = *first++;
        }
        return dest;
    }

    struct nothrow_t {
        /**
         * @brief Constructs a nothrow tag object.
         *
         * This tag is used to select overloads that are not expected to throw
         * exceptions.
         */
        explicit nothrow_t() noexcept;
    };

    extern nothrow_t nothrow;

    enum class align : std::size_t {};
} //namespace tlx

#endif //TLX_MEMORY_HPP