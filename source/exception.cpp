//
// Created by muham on 24.07.2026.
//

#include "tlx/exception.hpp"
#include <tlx/utility.hpp>

using namespace tlx;

Exception::Exception(const char *message) {
    this->m_message = message;
}

Exception::Exception(std::string message) {
    this->m_message = ::tlx::move(message);
}

Exception::~Exception() = default;

char const *Exception::what() const {
    return this->m_message.c_str();
}