//
// Created by muham on 27.07.2026.
//

#include <tlx/vector.hpp>
#include <tlx/random.hpp>
#include <tlx/types.hpp>
#include <cuda_runtime.h>
#include <iostream>

#define CUDA_CHECK(call)                                                     \
    do {                                                                     \
        cudaError_t err = (call);                                           \
        if (err != cudaSuccess) {                                           \
            fprintf(stderr, "CUDA error %s:%d: %s\n", __FILE__, __LINE__,   \
                    cudaGetErrorString(err));                                \
            std::abort();                                                   \
        }                                                                    \
    } while (0)

constexpr std::size_t CAP = 10;

__global__ void test_kernel(tlx::bfloat16* output, std::size_t* output_size,
                             const uint64_t seed) {
    tlx::vec<tlx::bfloat16, CAP> v;
    tlx::philox4x32 rng(seed);

    for (std::size_t i = 0; i < CAP; i++) {
        v.push(rng.next<tlx::bfloat16>());
    }

    for (std::size_t i = 0; i < v.size(); i++) {
        output[i] = v[i];
    }
    *output_size = v.size();
}

int main() {
    tlx::bfloat16* d_out;
    std::size_t* d_size;
    CUDA_CHECK(cudaMalloc(&d_out, CAP * sizeof(tlx::bfloat16)));
    CUDA_CHECK(cudaMalloc(&d_size, sizeof(std::size_t)));

    test_kernel<<<1, 1>>>(d_out, d_size, 1);
    CUDA_CHECK(cudaGetLastError());
    CUDA_CHECK(cudaDeviceSynchronize());

    tlx::bfloat16 h_out[CAP];
    std::size_t h_size;
    CUDA_CHECK(cudaMemcpy(h_out, d_out, CAP * sizeof(tlx::bfloat16), cudaMemcpyDeviceToHost));
    CUDA_CHECK(cudaMemcpy(&h_size, d_size, sizeof(std::size_t), cudaMemcpyDeviceToHost));

    std::cout << "size: " << h_size << std::endl;
    for (std::size_t i = 0; i < h_size; i++) {
        std::cout << h_out[i] << std::endl;
    }

    CUDA_CHECK(cudaFree(d_out));
    CUDA_CHECK(cudaFree(d_size));
    return 0;
}