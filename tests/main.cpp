//
// Created by muham on 26.07.2026.
//

#include <tlx/types.hpp>
#include <iostream>

int main() {
    const tlx::bfloat16 x1 = 1.123f;
    const tlx::bfloat16 x2 = 2.210f;

    if (x1 > x2) {
        std::cout << x1 << std::endl;
    } else if (x1 == x2) {
        std::cout << x1 << std::endl;
        std::cout << x2 << std::endl;
    } else {
        std::cout << x2 << std::endl;
    }

    return 0;
}