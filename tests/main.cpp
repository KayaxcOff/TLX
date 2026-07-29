//
// Created by muham on 26.07.2026.
//


#include <tlx/file_system.hpp>
#include <tlx/types.hpp>
#include <array>

#define N 10

int main() {
    tlx::fs::Hex hex;

    std::array<tlx::bfloat16, N> x;

    for (std::size_t i = 0; i < N; ++i) {
        x[i] = i;
    }

    hex.Set(
        reinterpret_cast<const std::byte *>(x.data()),
        N * sizeof(tlx::bfloat16),
        R"(C:\software\Cpp\projects\TLX\tests\output.hex)"
    );
    hex.write();

    return 0;
}