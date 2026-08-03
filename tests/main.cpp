//
// Created by muham on 26.07.2026.
//

#include <tlx/vector.hpp>
#include <tlx/random.hpp>
#include <tlx/types.hpp>
#include <iostream>

int main() {
    tlx::vec<tlx::bfloat16, 10> x(10);
    tlx::philox4x32 rng(1);

    for (auto& item : x) {
        item = rng.next<tlx::bfloat16>();
    }

    for (const auto& item : x) {
        std::cout << item << std::endl;
    }

    return 0;
}