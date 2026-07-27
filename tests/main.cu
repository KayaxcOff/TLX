//
// Created by muham on 27.07.2026.
//

#include <tlx/types.hpp>
#include <cuda_runtime.h>
#include <cassert>

using tlx::bfloat16;

#define N 1000

__global__
void add_kernel(const __nv_bfloat16 a, const __nv_bfloat16 b, bfloat16* c) {
    *c = a + b;
}

int main(int argc, char** argv) {
    const bfloat16 a = 1.0f;
    const bfloat16 b = 2.0f;

    bfloat16* d_out;
    cudaMalloc(&d_out, sizeof(bfloat16));

    add_kernel<<<1,1>>>(a, b, d_out);

    bfloat16 result;
    cudaMemcpy(&result, d_out, sizeof(result), cudaMemcpyDeviceToHost);

    cudaDeviceSynchronize();

    assert(static_cast<float>(result) == 3.0f);

    cudaFree(d_out);

    return 0;
}