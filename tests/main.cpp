//
// Created by muham on 26.07.2026.
//

#include <tlx/vector.hpp>
#include <tlx/float.hpp>
#include <iostream>

int main() {
    tlx::vec<tlx::half, 10> x1{1.f, 2.f, 3.f, 4.f};
    tlx::vec<float, 10> x2 = {1.f, 2.f, 3.f, 4.f};

    std::cout << "16 Bits Float" << std::endl;
    std::cout << "size of: " << sizeof(x1) << std::endl;
    std::cout << "capacity: " << x1.capacity() << std::endl;
    std::cout << "size: " << x1.size() << std::endl;
    std::cout << "data: {";
    for (const auto& item : x1) {
        std::cout << item << ", ";
    }
    std::cout << "}" << std::endl;

    std::cout << "32 Bits Float" << std::endl;
    std::cout << "size of: " << sizeof(x2) << std::endl;
    std::cout << "capacity: " << x2.capacity() << std::endl;
    std::cout << "size: " << x2.size() << std::endl;
    std::cout << "data: {";
    for (const auto& item : x2) {
        std::cout << item << ", ";
    }
    std::cout << "}" << std::endl;

    return 0;
}