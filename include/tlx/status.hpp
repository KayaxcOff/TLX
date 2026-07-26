//
// Created by muham on 26.07.2026.
//

#ifndef TLX_STATUS_HPP
#define TLX_STATUS_HPP

#include <cstdint>
#include <string>
#include <string_view>

namespace tlx {
    /**
     * @brief Enumeration of possible status / error codes used by the TLX library.
     */
    enum class StatusCode : std::uint8_t {
        Default = 0,
        Success = 1,
        OutOfMemory = 2,
        InvalidArgument = 3,
        Empty = 4,
        Zero = 5
    };

    /**
     * @brief Converts a StatusCode to a human-readable string.
     *
     * @param code Status code to convert.
     * @return std::string_view String representation of the status code.
     */
    [[nodiscard]]
    std::string_view ToString(StatusCode code);

    /**
     * @class Status
     * @brief Represents the result of an operation, including a status code and optional message.
     *
     * This class is used to report success or failure of operations in a
     * lightweight and expressive way.
     */
    class Status {
    public:
        /**
         * @brief Default constructor. Creates a Status with Default code.
         */
        Status();
        /**
         * @brief Constructs a Status with the given code and an empty message.
         *
         * @param code Status code.
         */
        explicit Status(StatusCode code);
        /**
         * @brief Constructs a Status with the given code and message.
         *
         * @param code Status code.
         * @param message Descriptive error / status message.
         */
        Status(StatusCode code, std::string message);
        Status(const Status& other);
        Status(Status&& other) noexcept;
        ~Status();

        /**
         * @brief Checks whether the status indicates success.
         *
         * @return true if the status code is StatusCode::Success.
         */
        [[nodiscard]]
        bool pass() const;
        /**
         * @brief Returns the status code.
         *
         * @return StatusCode The current status code.
         */
        [[nodiscard]]
        StatusCode code() const;
        /**
         * @brief Returns the associated message.
         *
         * @return std::string Copy of the status message.
         */
        [[nodiscard]]
        std::string message() const;
    private:
        StatusCode m_code;
        std::string m_message;
    };
} //namespace tlx

#endif //TLX_STATUS_HPP