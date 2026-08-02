//
// Created by muham on 26.07.2026.
//

#include <tlx/vector.hpp>
#include <tlx/string.hpp>
#include <iostream>

int main() {
    tlx::vec<tlx::vstring, 5> x1;

    x1.push("C++");
    x1.push("Python");
    x1.push("Java");
    x1.push("Rust");
    x1.push("C#");

    auto x2 = x1;

    for (const auto& item : x2) {
        std::cout << item << std::endl;
    }

    return 0;
}
/*
output:
C:\software\Cpp\projects\TLX\cmake-build-debug-visual-studio\TLX_HOST_TEST.exe
C++
Python
Java
Rust
C#

Process finished with exit code 0
*/