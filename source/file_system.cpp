//
// Created by muham on 28.07.2026.
//

#include "tlx/file_system.hpp"

using namespace tlx::fs;

Directory::Directory() {
    this->m_path = "";
}

Directory::Directory(const fs::path &path) {
    this->m_path = path;
}

Directory::Directory(const Directory &) = default;

Directory::Directory(Directory &&) noexcept = default;

Directory::~Directory() = default;

void Directory::New(const fs::path &path) {
    std::filesystem::create_directory(path);
}

void Directory::remove() const {
    std::filesystem::remove(this->m_path);
}

bool Directory::exist() const {
    return std::filesystem::exists(this->m_path);
}

const path &Directory::path() const {
    return this->m_path;
}