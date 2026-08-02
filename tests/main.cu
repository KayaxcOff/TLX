//
// Created by muham on 27.07.2026.
//

#include <tlx/vector.hpp>
#include <tlx/types.hpp>
#include <cuda_runtime.h>
#include <iostream>

__global__ void test_kernel(const __nv_bfloat16 x, std::size_t* output_size) {
    tlx::vec<tlx::bfloat16, 10> v;

    v.push(x);
    v.push(x);
    v.push(x);

    *output_size = v.size();
}

int main() {
    const tlx::bfloat16 v = 1.0f;
    std::size_t* d_out;
    cudaMalloc(&d_out, sizeof(std::size_t));

    test_kernel<<<1, 1>>>(v, d_out);

    std::size_t h_out;
    cudaMemcpy(&h_out, d_out, sizeof(std::size_t), cudaMemcpyDeviceToHost);

    std::cout << "output: " << h_out << std::endl;

    return 0;
}