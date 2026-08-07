//
// Created by muham on 26.07.2026.
//

#include "tlx/status.hpp"
#include <tlx/utility.hpp>

using namespace tlx;

std::string_view tlx::as_string(const StatusCode code) {
    switch (code) {
        case StatusCode::Default:
            return "default";
        case StatusCode::Success:
            return "success";
        case StatusCode::OutOfMemory:
            return "out of memory";
        case StatusCode::InvalidArgument:
            return "invalid argument";
        case StatusCode::Empty:
            return "empty";
        case StatusCode::Zero:
            return "zero";
        default:
            return "unknown";
    }
}

Status::Status() {
    this->m_code = StatusCode::Default;
}

Status::Status(const StatusCode code) {
    this->m_code = code;
}

Status::Status(const StatusCode code, std::string message) {
    this->m_code = code;
    this->m_message = ::tlx::move(message);
}

Status::Status(const Status &other) {
    this->m_code = other.m_code;
    this->m_message = other.m_message;
}

Status::Status(Status &&other) noexcept {
    this->m_code = other.m_code;
    this->m_message = ::tlx::move(other.m_message);
}

Status::~Status() = default;

bool Status::pass() const {
    return this->m_code == StatusCode::Success;
}

StatusCode Status::code() const {
    return this->m_code;
}

std::string_view Status::message() const {
    return this->m_message;
}