//
// Created by muham on 26.07.2026.
//

#include <tlx/vector.hpp>
#include <tlx/random.hpp>
#include <tlx/reference.hpp>
#include <tlx/types.hpp>
#include <iostream>

int main() {
    tlx::vec<tlx::bfloat16, 10> x(10);
    tlx::philox4x32 rng1(1);

    auto rng2 = tlx::ref(rng1);

    for (auto& item : x) {
        item = rng2->next<tlx::bfloat16>();
    }

    for (const auto& item : x) {
        std::cout << item << std::endl;
    }

    return 0;
}