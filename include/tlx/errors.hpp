//
// Created by muham on 2.08.2026.
//

#ifndef TLX_ERRORS_HPP
#define TLX_ERRORS_HPP

#include <tlx/macros.hpp>
#include <tlx/string.hpp>
#include <source_location>

namespace tlx {
    /**
     * @brief Conditionally terminates the program if the given condition is true.
     *
     * If `condition` evaluates to true, an error message is printed (including
     * source location information) and the program is terminated.
     *
     * @param condition Condition that triggers the exit when true.
     * @param msg       Error message to display.
     * @param loc       Source location of the call site (defaults to current location).
     */
    void exit_if(bool condition, vstring msg, const std::source_location& loc = std::source_location::current());
    /**
     * @brief Conditionally emits a warning if the given condition is true.
     *
     * If `condition` evaluates to true, a warning message is printed
     * (including source location information). The program continues execution.
     *
     * @param condition Condition that triggers the warning when true.
     * @param msg       Warning message to display.
     * @param loc       Source location of the call site (defaults to current location).
     */
    void warn_if(bool condition, vstring msg, const std::source_location& loc = std::source_location::current());

    /**
     * @brief Host/device compatible conditional exit.
     *
     * If `condition` is true, prints the file name and message to the console
     * and terminates the program with `TLX_ERROR_EXIT`. This function is
     * intended for use in both host and device (CUDA) code.
     *
     * @param condition Condition that triggers the exit when true.
     * @param msg       Error message to display.
     * @param file      Source file name (defaults to `__FILE__`).
     */
    TLX_HD inline void hd_exit(const bool condition, const vstring msg, const char* file = __FILE__) {
        if (condition) {
            printf("%s: %s\n", file, msg.c());
            std::exit(TLX_ERROR_EXIT);
        }
    }
} //namespace tlx

/**
 * @brief Convenience macro that calls `tlx::exit_if`.
 *
 * @param cond Condition that triggers the exit when true.
 * @param msg  Error message.
 */
#define TLX_EXIT_IF(cond, msg) \
    ::tlx::exit_if((cond), (msg))

/**
 * @brief Convenience macro that calls `tlx::warn_if`.
 *
 * @param cond Condition that triggers the warning when true.
 * @param msg  Warning message.
 */
#define TLX_WARN_IF(cond, msg) \
    ::tlx::warn_if((cond), (msg))

/**
 * @brief Convenience macro that calls `tlx::hd_exit` (host/device safe).
 *
 * @param cond Condition that triggers the exit when true.
 * @param msg  Error message.
 */
#define TLX_HD_ERROR(cond, msg) \
    ::tlx::hd_exit((cond), (msg))

#endif //TLX_ERRORS_HPP