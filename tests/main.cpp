//
// Created by muham on 26.07.2026.
//


#include <tlx/types.hpp>
#include <iostream>

int main() {
    const tlx::qint8 x1 = 10;

    std::cout << "value: " << x1 << std::endl;
    std::cout << "size: " << sizeof(x1) << " bytes" << std::endl;

    return 0;
}