//
// Created by muham on 28.07.2026.
//

#include "tlx/file_system.hpp"
#include <algorithm>

using namespace tlx::fs;

std::string tlx::fs::path(std::string _path) noexcept {
    std::ranges::replace(_path, '\\', '/');
    return _path;
}

Directory::Directory() = default;

Directory::Directory(const std::filesystem::path &path) {
    this->m_path = path;
}

Directory::Directory(const Directory &) = default;

Directory::Directory(Directory &&) noexcept = default;

Directory::~Directory() = default;

void Directory::Create(const std::filesystem::path &path) {
    std::filesystem::create_directory(path);
}

void Directory::Init() const {
    if (!std::filesystem::exists(this->m_path)) {
        std::filesystem::create_directory(this->m_path);
    }
}

void Directory::remove() const {
    std::filesystem::remove(this->m_path);
}

bool Directory::exists() const {
    return std::filesystem::exists(this->m_path);
}

const std::filesystem::path &Directory::path() const {
    return this->m_path;
}