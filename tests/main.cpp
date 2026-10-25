//
// Created by muham on 26.07.2026.
//

#include <tlx/vector.hpp>
#include <tlx/types.hpp>
#include <iostream>

int main() {
    tlx::vec<tlx::bfloat16, 10> x1{1.f, 2.f, 3.f, 4.f};

    std::cout << "capacity: " << x1.capacity() << std::endl;
    std::cout << "size: " << x1.size() << std::endl;
    std::cout << "data: {";
    for (const auto& item : x1) {
        std::cout << item << ", ";
    }
    std::cout << "}" << std::endl;

    return 0;
}