//
// Created by muham on 26.07.2026.
//

#include <tlx/vector.hpp>
#include <tlx/types.hpp>
#include <tlx/w.hpp>

int main() {
    tlx::vec<tlx::bfloat16, 5> x(5);

    for (auto& item : x) {
        item = 2;
    }

    for (const auto& item : x) {
        tlx::println("Value: {}", static_cast<float>(item));
    }

    return 0;
}