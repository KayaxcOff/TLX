//
// Created by muham on 26.07.2026.
//

#include <tlx/list.hpp>
#include <tlx/types.hpp>
#include <iostream>

int main() {
    tlx::list<tlx::bfloat16> x = {1.f, 2.f, 3.f, 4.f};

    for (const auto& item : x) {
        std::cout << item << std::endl;
    }

    return 0;
}