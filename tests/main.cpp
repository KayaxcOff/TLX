//
// Created by muham on 26.07.2026.
//


#include <tlx/span.hpp>
#include <tlx/types.hpp>
#include <iostream>

#define N 10

int main() {
    const auto x1 = new tlx::bfloat16[N];

    for (std::size_t i = 0; i < N; ++i) {
        x1[i] = static_cast<float>(i);
    }

    tlx::Span x2(x1, N);

    for (const auto& item : x2) {
        std::cout << item << std::endl;
    }

    delete[] x1;
}