//
// Created by muham on 24.07.2026.
//

#ifndef TLX_EXCEPTION_HPP
#define TLX_EXCEPTION_HPP

#include <exception>
#include <string>

namespace tlx {
    /**
     * @brief Base exception class for the TLX library.
     *
     * This class serves as the root of the TLX exception hierarchy.
     * It inherits from `std::exception` and provides convenient constructors
     * for string-based error messages.
     */
    class Exception : public std::exception {
    public:
        /**
         * @brief Constructs an exception with a C-style string message.
         *
         * @param message Null-terminated error message.
         */
        explicit Exception(const char* message);
        /**
         * @brief Constructs an exception with a std::string message.
         *
         * @param message Error message.
         */
        explicit Exception(std::string message);
        ~Exception() override;

        /**
         * @brief Returns the explanatory string.
         *
         * @return const char* Pointer to the error message.
         */
        [[nodiscard]]
        char const *what() const override;
    private:
        std::string m_message;
    };
} //namespace tlx

#endif //TLX_EXCEPTION_HPP