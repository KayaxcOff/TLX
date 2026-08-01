//
// Created by muham on 26.07.2026.
//


#include <tlx/types.hpp>
#include <tlx/vector.hpp>
#include <iostream>

int main() {
    tlx::vec<tlx::bfloat16, 10> x1(10);

    for (std::size_t i = 0; i < 10; ++i) {
        x1[i] = static_cast<float>(i) * 2.5f;
    }

    std::cout << alignof(tlx::bfloat16) << '\n';
    std::cout << alignof(std::size_t) << '\n';
    std::cout << "sizeof: " << sizeof(x1) << std::endl;
    std::cout << "size: " << x1.size() << std::endl;
    std::cout << "capacity: " << tlx::vec<tlx::bfloat16, 10>::capacity() << std::endl;
    std::cout << "data: " << x1.data() << std::endl;
    std::cout << "{ ";
    for (const auto& item : x1) {
        std::cout << item << ", ";
    }
    std::cout << "}" << std::endl;

    return 0;
}