//
// Created by muham on 2.08.2026.
//

#include "tlx/errors.hpp"
#ifdef _WIN32
    #include <windows.h>
#endif //#ifdef _WIN32

namespace {
    void enable_ansi_colors() {
        #ifdef _WIN32
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

            if (hOut == INVALID_HANDLE_VALUE)
                return;

            DWORD mode = 0;
            if (!GetConsoleMode(hOut, &mode))
                return;

            mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, mode);
        #endif //#ifdef _WIN32
    }
} //namespace

void tlx::exit_if(const bool condition, const vstring msg, const std::source_location &loc) {
    if (condition) {
        enable_ansi_colors();
        std::printf(
            "\033[31m[%s:%u] | %s\n",
            loc.file_name(),
            loc.line(),
            msg.c()
        );
        std::exit(TLX_ERROR_EXIT);
    }
}

void tlx::warn_if(const bool condition, const vstring msg, const std::source_location &loc) {
    if (condition) {
        enable_ansi_colors();
        std::printf(
            "\033[33m[%s:%u] | %s\n",
            loc.file_name(),
            loc.line(),
            msg.c()
        );
    }
}
