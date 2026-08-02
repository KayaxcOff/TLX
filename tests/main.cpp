//
// Created by muham on 26.07.2026.
//

#include <tlx/vector.hpp>
#include <tlx/string.hpp>
#include <iostream>

int main() {
    tlx::vec<tlx::vstring, 5> x;

    x.push("C++");
    x.push("Python");
    x.push("Java");
    x.push("Rust");
    x.push("C#");
    //x.push("JavaScript");

    for (const auto& item : x) {
        std::cout << item << std::endl;
    }

    return 0;
}