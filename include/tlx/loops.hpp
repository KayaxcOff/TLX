//
// Created by muham on 19.08.2026.
//

#ifndef TLX_LOOPS_HPP
#define TLX_LOOPS_HPP

#include <tlx/utility.hpp>

namespace tlx {
    template<typename T>
    void If(const bool condition, T&& t) {
        if (condition) {
            ::tlx::forward<T>(t)();
        }
    }
    template <typename F>
    void For(const int start, const int end, F&& action) {
        for (int i = start; i < end; ++i) {
            action(i);
        }
    }
} //namespace tlx

#endif //TLX_LOOPS_HPP