//
// Created by muham on 26.07.2026.
//

#include <tlx/memory.hpp>
#include <tlx/reference.hpp>
#include <iostream>
#include <string>

struct Printer {
    Printer() {
        this->message = "Hello World";
    }

    void print() const {
        std::cout << this->message << std::endl;
    }
private:
    std::string message;
};

int main() {
    const Printer x1;

    auto x2 = tlx::cref(x1);

    if (tlx::isSameAddress(x1, x2())) {
        std::cout << "They are the same" << std::endl;
    } else {
        std::cout << "They are not the same" << std::endl;
    }

    return 0;
}