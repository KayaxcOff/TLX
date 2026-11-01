//
// Created by muham on 26.07.2026.
//

#include <tlx/quantize_types.hpp>
#include <iostream>

int main() {
    const tlx::qint16 x1 = 1;

    std::cout << "value: " << x1 << std::endl;
    std::cout << "size of: " << sizeof(x1) << " bytes" << std::endl;
    std::cout << "align of: " << alignof(tlx::qint16) << " bytes" << std::endl;

    return 0;
}