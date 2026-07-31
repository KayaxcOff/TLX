//
// Created by muham on 28.07.2026.
//

#ifndef TLX_FILE_SYSTEM_HPP
#define TLX_FILE_SYSTEM_HPP

#include <filesystem>
#include <string>

namespace tlx::fs {
    /**
     * @brief Normalizes a path string by replacing all backslashes with forward slashes.
     *
     * This function converts Windows-style path separators (`\`) to POSIX-style
     * separators (`/`). It is useful for creating portable path strings.
     *
     * @param _path Input path string (will be modified and returned).
     * @return std::string The normalized path with forward slashes.
     */
    [[nodiscard]]
    std::string path(std::string _path) noexcept;

    /**
     * @brief A simple wrapper for directory operations.
     *
     * This class represents a directory path and provides basic operations
     * such as creation, removal, and existence checking. It does not own
     * the underlying filesystem resources beyond the path itself.
     */
    class Directory {
    public:
        Directory();
        explicit Directory(const std::filesystem::path& path);
        Directory(const Directory&);
        Directory(Directory&&) noexcept;
        ~Directory();

        /**
         * @brief Creates a directory at the specified path.
         *
         * Equivalent to `std::filesystem::create_directory`.
         *
         * @param path Path where the directory should be created.
         */
        static void Create(const std::filesystem::path& path);

        /**
         * @brief Creates directory to path
         */
        void Init() const;
        /**
         * @brief Checks whether the directory exists.
         *
         * @return true if the path exists and is a directory.
         */
        void remove() const;
        /**
         * @brief Checks whether the directory exists.
         *
         * @return true if the path exists and is a directory.
         */
        [[nodiscard]]
        bool exists() const;
        /**
         * @brief Returns the path of the directory.
         *
         * @return const path& Reference to the stored path.
         */
        [[nodiscard]]
        const std::filesystem::path& path() const;
    private:
        std::filesystem::path m_path;
    };

    /**
     * @brief Utility class for writing binary data as hexadecimal text.
     *
     * This class stores a non-owning view of a byte buffer together with an
     * optional output path. It can write the buffer contents as a human-readable
     * hexadecimal dump (16 bytes per line) either immediately or later via
     * the member `write()` function.
     */
    class Hex {
    public:
        /**
         * @brief Default constructor. Creates an empty Hex object.
         */
        Hex();
        /**
         * @brief Constructs a Hex object from a byte buffer (no output path).
         *
         * @param data Pointer to the beginning of the byte buffer.
         * @param size Number of bytes in the buffer.
         */
        Hex(const std::byte* data, std::size_t size);
        /**
         * @brief Constructs a Hex object from a byte buffer and an output path.
         *
         * @param data Pointer to the beginning of the byte buffer.
         * @param size Number of bytes in the buffer.
         * @param path Filesystem path where the hexadecimal dump will be written.
         */
        Hex(const std::byte* data, std::size_t size, const std::filesystem::path& path);
        Hex(const Hex&);
        Hex(Hex&&) noexcept;
        ~Hex();

        /**
         * @brief Writes a byte buffer as a hexadecimal dump to the given file.
         *
         * The output format is 16 bytes per line, each byte represented by
         * two hexadecimal digits separated by spaces.
         *
         * @param data Pointer to the beginning of the byte buffer.
         * @param size Number of bytes in the buffer.
         * @param path Output file path.
         */
        static void write(const std::byte* data, std::size_t size, const std::filesystem::path& path);

        /**
         * @brief Sets the internal buffer and output path.
         *
         * @param data Pointer to the beginning of the byte buffer.
         * @param size Number of bytes in the buffer.
         * @param path Filesystem path where the hexadecimal dump will be written.
         */
        void Set(const std::byte* data, std::size_t size, const std::filesystem::path& path);
        /**
         * @brief Writes the currently stored buffer to the stored path.
         *
         * Equivalent to calling the static `write()` with the internal members.
         */
        void write() const;

        [[nodiscard]]
        const std::filesystem::path& path() const;

        Hex& operator=(const Hex&);
        Hex& operator=(Hex&&) noexcept;
    private:
        const std::byte* m_data;
        std::size_t m_size;
        std::filesystem::path m_path;
    };

    /**
     * @brief Utility class for reading and writing raw binary data to/from files.
     *
     * This class stores a non-owning pointer to a byte buffer together with an
     * optional file path. It provides both static and member functions for
     * binary read/write operations. The actual I/O is delegated to internal
     * helper classes (`BinaryWriter` / `BinaryReader`).
     */
    class Finger12 {
    public:
        Finger12();
        /**
         * @brief Constructs a Finger12 from a byte buffer (no path).
         *
         * @param data Pointer to the beginning of the byte buffer.
         * @param size Number of bytes in the buffer.
         */
        Finger12(std::byte* data, std::size_t size);
        /**
         * @brief Constructs a Finger12 from a byte buffer and a file path.
         *
         * @param data Pointer to the beginning of the byte buffer.
         * @param size Number of bytes in the buffer.
         * @param path Filesystem path used for subsequent read/write operations.
         */
        Finger12(std::byte* data, std::size_t size, const std::filesystem::path& path);
        Finger12(const Finger12&);
        Finger12(Finger12&&) noexcept;
        ~Finger12();

        /**
         * @brief Writes a byte buffer to a file in binary mode.
         *
         * @param data Pointer to the beginning of the byte buffer.
         * @param size Number of bytes to write.
         * @param path Destination file path.
         */
        static void write(const std::byte* data, std::size_t size, const std::filesystem::path& path);
        /**
         * @brief Reads binary data from a file into a byte buffer.
         *
         * @param data Pointer to the destination buffer.
         * @param size Number of bytes to read.
         * @param path Source file path.
         */
        static void read(std::byte* data, std::size_t size, const std::filesystem::path& path);

        /**
         * @brief Sets the internal buffer pointer, size and file path.
         *
         * @param data Pointer to the beginning of the byte buffer.
         * @param size Number of bytes in the buffer.
         * @param path Filesystem path used for subsequent read/write operations.
         */
        void Set(std::byte* data, std::size_t size, const std::filesystem::path& path);
        /**
         * @brief Writes the currently stored buffer to the stored path.
         *
         * Equivalent to calling the static `write()` with the internal members.
         */
        void write() const;
        /**
         * @brief Reads data from the stored path into the currently stored buffer.
         *
         * Equivalent to calling the static `read()` with the internal members.
         */
        void read() const;

        /**
         * @brief Returns the currently stored file path.
         *
         * @return const std::filesystem::path& Reference to the stored path.
         */
        [[nodiscard]]
        const std::filesystem::path& path() const;

        Finger12& operator=(const Finger12&);
        Finger12& operator=(Finger12&&) noexcept;
    private:
        std::byte* m_data;
        std::size_t m_size;
        std::filesystem::path m_path;
    };
} //namespace tlx::fs

#endif //TLX_FILE_SYSTEM_HPP