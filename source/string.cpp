//
// Created by muham on 28.07.2026.
//

#include "tlx/string.hpp"

namespace tlx {
    std::ostream& operator<<(std::ostream& os, const vstring& value) {
        os.write(value.c(), static_cast<std::int64_t>(value.size()));
        return os;
    }
} //namespace tlx