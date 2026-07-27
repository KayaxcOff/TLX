//
// Created by muham on 26.07.2026.
//

#include <tlx/buffer.hpp>
#include <tlx/types.hpp>
#include <iostream>

using namespace tlx;
#define N 100

int main() {
    auto* x1 = new bfloat16[N];

    for (std::size_t i = 0; i < N; i++) {
        x1[i] = static_cast<float>(i);
    }

    BufferView bv(reinterpret_cast<std::byte*>(x1), N);

    for (std::size_t i = 0; i < N / 10; i++) {
        std::cout << bv.as<bfloat16>()[i] << std::endl;
    }

    delete[] x1;

    return 0;
}