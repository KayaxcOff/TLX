//
// Created by muham on 26.07.2026.
//

#include <tlx/loops.hpp>
#include <tlx/types.hpp>
#include <iostream>

int main() {
    const tlx::bfloat16 x1 = 1.f;
    const tlx::bfloat16 x2 = 2.f;

    tlx::If(x2 > x1, [&] {
        std::cout << "inside x1: " << x1 << std::endl;
        std::cout << "inside x2: " << x2 << std::endl;
    });

    return 0;
}