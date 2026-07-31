//
// Created by muham on 26.07.2026.
//


#include <tlx/types.hpp>
#include <iostream>

int main() {
    const tlx::quint8 x = 10;

    std::cout << "value: " << x << std::endl;
    std::cout << "size: " << sizeof(x) << std::endl;

    return 0;
}