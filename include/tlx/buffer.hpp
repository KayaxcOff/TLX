//
// Created by muham on 27.07.2026.
//

#ifndef TLX_BUFFER_HPP
#define TLX_BUFFER_HPP

#include <cstddef>

namespace tlx {
    /**
     * @brief A non-owning view over a contiguous sequence of bytes.
     *
     * `BufferView` stores a pointer to the beginning of a memory region
     * and its size in bytes. It does not own the memory and therefore
     * does not allocate or free it. Move operations are deleted to
     * emphasize the non-owning nature of the class.
     */
    class BufferView {
    public:
        /**
         * @brief Default constructor. Creates an empty view.
         */
        BufferView();
        /**
         * @brief Constructs a view over the given memory region.
         *
         * @param data Pointer to the start of the buffer.
         * @param size Size of the buffer in bytes.
         */
        BufferView(std::byte* data, std::size_t size);
        BufferView(const BufferView& other);
        BufferView(BufferView&& other) noexcept;
        ~BufferView();

        /**
         * @brief Reinterprets the buffer as a pointer to type T.
         *
         * @tparam T Desired element type.
         * @return T* Pointer to the first element of type T.
         */
        template<typename T>
        [[nodiscard]]
        T* as() {
            return reinterpret_cast<T*>(this->m_data);
        }
        /**
         * @brief Reinterprets the buffer as a const pointer to type T.
         *
         * @tparam T Desired element type.
         * @return const T* Const pointer to the first element of type T.
         */
        template<typename T>
        [[nodiscard]]
        const T* as() const {
            return reinterpret_cast<const T*>(this->m_data);
        }
        [[nodiscard]]
        std::byte* raw() noexcept;
        /**
         * @brief Returns a raw pointer to the underlying byte data.
         *
         * @return std::byte* Pointer to the beginning of the buffer.
         */
        [[nodiscard]]
        std::byte* raw() const noexcept;
        /**
         * @brief Returns the size of the buffer in bytes.
         *
         * @return std::size_t Size of the viewed memory region.
         */
        [[nodiscard]]
        std::size_t size() const noexcept;
        /**
         * @brief Checks whether the buffer is empty.
         *
         * @return true if the size is zero.
         */
        [[nodiscard]]
        bool empty() const noexcept;

        BufferView& operator=(const BufferView& other);
        BufferView& operator=(BufferView&& other) noexcept;
    private:
        std::byte* m_data;
        std::size_t m_size;
    };
} //namespace tlx

#endif //TLX_BUFFER_HPP