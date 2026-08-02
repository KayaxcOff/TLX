//
// Created by muham on 26.07.2026.
//

#include <tlx/vector.hpp>
#include <iostream>
#include <string>

int main() {
    tlx::vec<std::string, 3> x;

    x.emplace("C++");
    x.emplace("Python");
    x.emplace("Java");

    for (const auto& item : x) {
        std::cout << item << std::endl;
    }

    return 0;
}