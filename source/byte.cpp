//
// Created by muham on 27.07.2026.
//

#include "tlx/buffer.hpp"
#include <tlx/utility.hpp>

using namespace tlx;

BufferView::BufferView() {
    this->m_data = nullptr;
    this->m_size = 0;
}

BufferView::BufferView(std::byte *data, const std::size_t size) {
    this->m_data = data;
    this->m_size = size;
}

BufferView::BufferView(const BufferView &other) {
    this->m_data = other.m_data;
    this->m_size = other.m_size;
}

BufferView::~BufferView() = default;

std::byte *BufferView::raw() noexcept {
    return this->m_data;
}

std::byte *BufferView::raw() const noexcept {
    return this->m_data;
}

std::size_t BufferView::size() const noexcept {
    return this->m_size;
}

bool BufferView::empty() const noexcept {
    return this->m_size == 0;
}

BufferView &BufferView::operator=(const BufferView &other) {
    if (this != &other) {
        this->m_data = other.m_data;
        this->m_size = other.m_size;
    }
    return *this;
}