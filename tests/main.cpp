//
// Created by muham on 26.07.2026.
//

#include <tlx/string.hpp>
#include <iostream>

int main() {
    const tlx::vstring x1 = "Hello World!";

    std::cout << x1.ToString() << std::endl;

    return 0;
}