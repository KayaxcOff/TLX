//
// Created by muham on 26.07.2026.
//

#include "tlx/types.hpp"

namespace tlx {
    std::ostream& operator<<(std::ostream& os, const bfloat16& value) {
        os << std::fixed << static_cast<float>(value);
        return os;
    }
} //namespace tlx