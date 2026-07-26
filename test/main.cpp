//
// Created by muham on 26.07.2026.
//

#include <tlx/types.hpp>
#include <iostream>

int main() {
    const tlx::bfloat16 x1 = 2.0f;

    std::cout << "value: " << x1 << std::endl;
    std::cout << "size: " << sizeof(x1) << " bytes" << std::endl;

    return 0;
}