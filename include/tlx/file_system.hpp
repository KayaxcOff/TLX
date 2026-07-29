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
} //namespace tlx::fs

#endif //TLX_FILE_SYSTEM_HPP