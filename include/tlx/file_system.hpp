//
// Created by muham on 28.07.2026.
//

#ifndef TLX_FILE_SYSTEM_HPP
#define TLX_FILE_SYSTEM_HPP

#include <filesystem>

namespace tlx::fs {
    using path = std::filesystem::path;

    class Directory {
    public:
        Directory();
        explicit Directory(const path& path);
        Directory(const Directory&);
        Directory(Directory&&) noexcept;
        ~Directory();

        static void New(const path& path);

        void remove() const;
        [[nodiscard]]
        bool exist() const;
        [[nodiscard]]
        const path& path() const;
    private:
        fs::path m_path;
    };
} //namespace tlx::fs

#endif //TLX_FILE_SYSTEM_HPP