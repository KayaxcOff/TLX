//
// Created by muham on 28.07.2026.
//

#include "tlx/file_system.hpp"
#include <tlx/exception.hpp>
#include <algorithm>
#include <fstream>

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

Hex::Hex() {
    this->m_data = nullptr;
    this->m_size = 0;
}

Hex::Hex(const std::byte *data, const std::size_t size) {
    this->m_data = data;
    this->m_size = size;
}

Hex::Hex(const std::byte *data, const std::size_t size, const std::filesystem::path &path) {
    this->m_data = data;
    this->m_size = size;
    this->m_path = path;
}

Hex::Hex(const Hex &) = default;

Hex::Hex(Hex &&) noexcept = default;

Hex::~Hex() = default;

void Hex::write(const std::byte *data, const std::size_t size, const std::filesystem::path &path) {
    if (!data) {
        throw Exception("Data is nullptr");
    }

    std::ofstream file(path);

    if (!file) {
        throw Exception("File is nullptr");
    }

    file << std::hex << std::setfill('0');

    for (std::size_t i = 0; i < size; ++i) {
        file << std::setw(2) << static_cast<unsigned>(data[i]);

        if ((i + 1) % 16 == 0) {
            file << '\n';
        } else if (i + 1 != size) {
            file << ' ';
        }
    }
}

void Hex::Set(const std::byte *data, const std::size_t size, const std::filesystem::path &path) {
    this->m_data = data;
    this->m_size = size;
    this->m_path = path;
}

void Hex::write() const {
    write(this->m_data, this->m_size, this->m_path);
}

const std::filesystem::path &Hex::path() const {
    return this->m_path;
}

Hex &Hex::operator=(const Hex &) = default;

Hex &Hex::operator=(Hex &&) noexcept = default;

namespace {
    class [[maybe_unused]] BinaryWriter {
    public:
        BinaryWriter() = default;
        ~BinaryWriter() = default;

        static void write(const std::byte *data, const std::size_t size, const std::filesystem::path &path) {
            std::ofstream file(path, std::ios::binary);
            if (!file) {
                throw tlx::Exception("File is nullptr");
            }

            file.write(
                reinterpret_cast<const char*>(data),
                static_cast<std::streamsize>(size)
            );
        }
    };

    class [[maybe_unused]] BinaryReader {
    public:
        BinaryReader() = default;
        ~BinaryReader() = default;

        static void read(std::byte *data, const std::size_t size, const std::filesystem::path &path) {
            std::ifstream file(path, std::ios::binary);

            if (!file) {
                throw tlx::Exception("File is nullptr");
            }

            file.read(
                reinterpret_cast<char*>(data),
                static_cast<std::streamsize>(size)
            );
        }
    };
} //unnamed namespace

Finger12::Finger12() {
    this->m_data = nullptr;
    this->m_size = 0;
}

Finger12::Finger12(std::byte *data, const std::size_t size) {
    this->m_data = data;
    this->m_size = size;
}

Finger12::Finger12(std::byte *data, const std::size_t size, const std::filesystem::path &path) {
    this->m_data = data;
    this->m_size = size;
    this->m_path = path;
}

Finger12::Finger12(const Finger12 &) = default;

Finger12::Finger12(Finger12 &&) noexcept = default;

Finger12::~Finger12() = default;

void Finger12::write(const std::byte *data, const std::size_t size, const std::filesystem::path &path) {
    BinaryWriter::write(
        data,
        size,
        path
    );
}

void Finger12::read(std::byte *data, const std::size_t size, const std::filesystem::path &path) {
    BinaryReader::read(
        data,
        size,
        path
    );
}

void Finger12::Set(std::byte *data, const std::size_t size, const std::filesystem::path &path) {
    this->m_data = data;
    this->m_size = size;
    this->m_path = path;
}

void Finger12::write() const {
    BinaryWriter::write(this->m_data, this->m_size, this->m_path);
}

void Finger12::read() const {
    BinaryReader::read(this->m_data, this->m_size, this->m_path);
}

const std::filesystem::path &Finger12::path() const {
    return this->m_path;
}

Finger12 &Finger12::operator=(const Finger12 &) = default;

Finger12 &Finger12::operator=(Finger12 &&) noexcept = default;
