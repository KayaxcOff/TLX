//
// Created by muham on 29.08.2026.
//

#ifndef TLX_W_HPP
#define TLX_W_HPP

#include <format>
#include <iostream>
#include <string_view>

namespace tlx {
    inline void println(const std::string_view text) {
        std::cout << text << '\n';
    }

    template<class... Args>
    void println(std::format_string<Args...> fmt, Args&&... args) {
        std::cout << std::format(fmt, std::forward<Args>(args)...) << '\n';
    }
} //namespace tlx

#endif //TLX_W_HPP