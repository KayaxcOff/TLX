//
// Created by muham on 3.08.2026.
//

#ifndef TLX_RANDOM_HPP
#define TLX_RANDOM_HPP

#include <tlx/concepts.hpp>

namespace tlx {

    /**
     * @brief Counter-based pseudo-random number generator (Philox4x32-10).
     *
     * Stateless-friendly, deterministic RNG suitable for both host and device
     * code. Each `(seed, subsequence, offset)` triple deterministically maps
     * to an independent stream, making it well-suited for parallel generation
     * across GPU threads without synchronization.
     */
    class philox4x32 {
    public:
        /**
         * @brief Constructs a generator for a given seed, subsequence, and offset.
         *
         * @param seed        Global seed shared across all streams.
         * @param subsequence Independent stream index (e.g. thread/element index).
         * @param offset      Starting position within the stream (for skip-ahead).
         */
        TLX_HD explicit philox4x32(const std::uint64_t seed, const std::uint64_t subsequence = 0, const std::uint64_t offset = 0) noexcept {
            this->m_key[0] = static_cast<std::uint32_t>(seed);
            this->m_key[1] = static_cast<std::uint32_t>(seed >> 32);

            this->m_counter[0] = static_cast<std::uint32_t>(offset);
            this->m_counter[1] = static_cast<std::uint32_t>(offset >> 32);
            this->m_counter[2] = static_cast<std::uint32_t>(subsequence);
            this->m_counter[3] = static_cast<std::uint32_t>(subsequence >> 32);

            this->m_buffer = philox4x32_output{};
            this->m_buffer_index = 4; // force generation on first next()
        }

        /**
         * @brief Returns the next raw random value as type `T`.
         *
         * For integral `T`, returns raw generated bits truncated/widened to
         * fit `T`. For floating-point `T` (including bfloat16/half), the bits
         * are combined into a value uniformly distributed in [0, 1) using the
         * standard mantissa-injection technique.
         *
         * @tparam T Arithmetic type of the result.
         * @return T Next generated value.
         */
        template<arithmetic_like T>
        [[nodiscard]]
        TLX_HD T next() noexcept {
            if constexpr (integral<T>) {
                if constexpr (sizeof(T) <= 4) {
                    return static_cast<T>(next_u32());
                } else {
                    const std::uint64_t lo = next_u32();
                    const std::uint64_t hi = next_u32();
                    return static_cast<T>((hi << 32) | lo);
                }
            } else {
                if constexpr (sizeof(T) <= 4) {
                    return u32_to_unit_float<T>(next_u32());
                } else {
                    const std::uint32_t lo = next_u32();
                    const std::uint32_t hi = next_u32();
                    return u64_to_unit_double((static_cast<std::uint64_t>(hi) << 32) | lo);
                }
            }
        }

        /**
         * @brief Returns a uniformly distributed value.
         *
         * For floating-point `T`, the result lies in [0, 1). For integral
         * `T`, the result spans the full range of `T` (uniform over all
         * representable values, subject to standard modulo-based generation).
         *
         * @tparam T Arithmetic type of the result.
         * @return T Uniformly distributed value.
         */
        template<arithmetic_like T>
        [[nodiscard]]
        TLX_HD T uniform() noexcept {
            return next<T>();
        }

        /**
         * @brief Returns a uniformly distributed value in the half-open range [lo, hi).
         *
         * @tparam T  Arithmetic type of the result.
         * @param lo Inclusive lower bound.
         * @param hi Exclusive upper bound.
         * @return T Uniformly distributed value in [lo, hi).
         */
        template<arithmetic_like T>
        [[nodiscard]]
        TLX_HD T uniform(const T lo, const T hi) noexcept {
            TLX_HD_ERROR(hi <= lo, "uniform: hi must be greater than lo");
            if constexpr (float_like<T>) {
                return lo + next<T>() * (hi - lo);
            } else {
                const auto range = static_cast<std::uint64_t>(hi) - static_cast<uint64_t>(lo);
                return static_cast<T>(lo + static_cast<std::uint64_t>(next_u32()) % range);
            }
        }

        /**
         * @brief Advances the generator by `n` outputs without returning them.
         *
         * @param n Number of raw 32-bit outputs to skip.
         */
        TLX_HD void discard(std::uint64_t n) noexcept {
            const std::uint64_t available = 4 - this->m_buffer_index;
            if (n <= available) {
                this->m_buffer_index += static_cast<int>(n);
                return;
            }
            n -= available;
            const std::uint64_t blocks = n / 4;
            this->bump_counter(blocks);
            this->m_buffer = generate_block();
            this->m_buffer_index = static_cast<int>(n % 4);
        }

    private:
        /**
         * @brief 4x32-bit output block produced by a single Philox round sequence.
         */
        struct philox4x32_output {
            uint32_t v[4];
        };

        /**
         * @brief Computes the high and low 32 bits of a 32x32-bit product.
         */
        TLX_HD static void mulhilo32(const std::uint32_t a, const std::uint32_t b, std::uint32_t& hi, std::uint32_t& lo) noexcept {
            #if defined(__CUDA_ARCH__)
                lo = a * b;
                hi = __umulhi(a, b);
            #else //#if defined(__CUDA_ARCH__)
                const std::uint64_t product = static_cast<std::uint64_t>(a) * static_cast<std::uint64_t>(b);
                lo = static_cast<std::uint32_t>(product);
                hi = static_cast<std::uint32_t>(product >> 32);
            #endif //#if defined(__CUDA_ARCH__) #else
        }

        /**
         * @brief Runs the 10-round Philox4x32 permutation on the current counter/key.
         */
        [[nodiscard]]
        TLX_HD philox4x32_output generate_block() const noexcept {
            std::uint32_t c0 = this->m_counter[0];
            std::uint32_t c1 = this->m_counter[1];
            std::uint32_t c2 = this->m_counter[2];
            std::uint32_t c3 = this->m_counter[3];
            std::uint32_t k0 = this->m_key[0];
            std::uint32_t k1 = this->m_key[1];

            #if defined(__CUDA_ARCH__)
                #pragma unroll
            #endif //#if defined(__CUDA_ARCH__)
            for (int round = 0; round < 10; round++) {
                std::uint32_t hi0, lo0, hi1, lo1;
                mulhilo32(PHILOX_M4x32_0, c0, hi0, lo0);
                mulhilo32(PHILOX_M4x32_1, c2, hi1, lo1);

                const std::uint32_t new_c0 = hi1 ^ c1 ^ k0;
                const std::uint32_t new_c1 = lo1;
                const std::uint32_t new_c2 = hi0 ^ c3 ^ k1;
                const std::uint32_t new_c3 = lo0;

                c0 = new_c0;
                c1 = new_c1;
                c2 = new_c2;
                c3 = new_c3;

                k0 += PHILOX_W32_0;
                k1 += PHILOX_W32_1;
            }

            return philox4x32_output{c0, c1, c2, c3};
        }

        /**
         * @brief Increments the 128-bit counter by `n` (default 1).
         */
        TLX_HD void bump_counter(std::uint64_t n = 1) noexcept {
            while (n > 0) {
                if (++this->m_counter[0] == 0) {
                    if (++this->m_counter[1] == 0) {
                        if (++this->m_counter[2] == 0) {
                            ++this->m_counter[3];
                        }
                    }
                }
                n--;
            }
        }

        /**
         * @brief Returns the next raw 32-bit output, refilling the block if needed.
         */
        TLX_HD std::uint32_t next_u32() noexcept {
            if (this->m_buffer_index >= 4) {
                this->m_buffer = generate_block();
                bump_counter();
                this->m_buffer_index = 0;
            }
            return this->m_buffer.v[this->m_buffer_index++];
        }

        /**
         * @brief Converts raw bits into a value uniformly distributed in [0, 1).
         */
        template<float_like T>
        [[nodiscard]]
        TLX_HD static T u32_to_unit_float(const std::uint32_t bits) noexcept {
            constexpr std::uint32_t mantissa_bits = 24;
            constexpr float scale = 1.0f / static_cast<float>(1u << mantissa_bits);
            return static_cast<T>(static_cast<float>(bits >> (32 - mantissa_bits)) * scale);
        }

        /**
         * @brief Converts raw 64-bit input into a `double` uniformly distributed in [0, 1).
         */
        [[nodiscard]]
        TLX_HD static double u64_to_unit_double(const std::uint64_t bits) noexcept {
            constexpr std::uint32_t mantissa_bits = 53;
            constexpr double scale = 1.0 / static_cast<double>(1ull << mantissa_bits);
            return static_cast<double>(bits >> (64 - mantissa_bits)) * scale;
        }

        static constexpr std::uint32_t PHILOX_M4x32_0 = 0xD2511F53u;
        static constexpr std::uint32_t PHILOX_M4x32_1 = 0xCD9E8D57u;
        static constexpr std::uint32_t PHILOX_W32_0   = 0x9E3779B9u;
        static constexpr std::uint32_t PHILOX_W32_1   = 0xBB67AE85u;

        std::uint32_t m_counter[4]{};
        std::uint32_t m_key[2]{};
        philox4x32_output m_buffer{};
        std::int32_t m_buffer_index;
    };
} //namespace tlx

#endif //TLX_RANDOM_HPP