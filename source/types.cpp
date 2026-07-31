//
// Created by muham on 26.07.2026.
//

#include "tlx/types.hpp"

namespace tlx {
    std::ostream& operator<<(std::ostream& os, const bfloat16& value) {
        os << std::fixed << static_cast<float>(value);
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const half& value) {
        os << std::fixed << static_cast<float>(value);
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const qint16& value) {
        os << std::fixed << static_cast<std::int32_t>(value);
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const qint8& value) {
        os << std::fixed << static_cast<std::int32_t>(value);
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const quint16& value) {
        os << std::fixed << static_cast<std::uint32_t>(value);
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const quint8& value) {
        os << std::fixed << static_cast<std::uint32_t>(value);
        return os;
    }
} //namespace tlx