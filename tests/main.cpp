//
// Created by muham on 26.07.2026.
//

#include <tlx/file_system.hpp>
#include <iostream>

using namespace tlx;

int main() {
    const fs::Directory directory(fs::path(R"(C:\software\Cpp\projects\TLX\tests\output)"));
    directory.Init();

    if (directory.exists()) {
        std::cout << "Success!" << std::endl;
    } else {
        std::cout << "Failed!" << std::endl;
    }

    return 0;
}