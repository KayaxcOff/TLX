//
// Created by muham on 18.10.2026.
//

#ifndef TLX_DETAIL_ERROR_HPP
#define TLX_DETAIL_ERROR_HPP

#include <tlx/_detail/flags.hpp>
#include <tlx/string.hpp>

namespace tlx {
    TLX_HD
    inline void exitIf(const vstring msg, const char* file = __FILE__) {
        std::printf("%s: %s\n", file, msg.c());
        std::exit(TLX_ERROR_EXIT);
    }
} //namespace tlx

#endif //TLX_DETAIL_ERROR_HPP