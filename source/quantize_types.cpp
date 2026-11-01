//
// Created by muham on 1.11.2026.
//

#include "tlx/quantize_types.hpp"

namespace tlx {
    std::ostream& operator<<(std::ostream& os, const qint8& value) {
        os << std::fixed << static_cast<std::int32_t>(value);
        return os;
    }
    std::ostream& operator<<(std::ostream& os, const qint16& value) {
        os << std::fixed << static_cast<std::int32_t>(value);
        return os;
    }
} //namespace tlx