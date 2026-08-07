# TLX

<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-20-blue.svg?style=flat-for-the-badge&logo=c%2B%2B" alt="C++20" />
  <img src="https://img.shields.io/badge/CUDA-Enabled-green.svg?style=flat-for-the-badge&logo=nvidia" alt="CUDA Support" />
  <img src="https://img.shields.io/badge/Build-CMake-orange.svg?style=flat-for-the-badge&logo=cmake" alt="CMake" />
  <img src="https://img.shields.io/badge/License-MIT-yellow.svg?style=flat-for-the-badge" alt="MIT License" />
</p>

TLX is a modern, high-performance, modular, and lightweight utility library built from the ground up using the **C++20** standard. It provides zero-overhead abstractions for large-scale and performance-critical applications, particularly in machine learning and high-performance computing (HPC).

Originally developed as the core utility library for the **CortexMind** machine learning framework, TLX can be easily integrated into existing C++ projects without requiring any external dependencies.

---

## Key Features

* **Modern C++20:** Utilizes Concepts, safe `std::format`-style formatting, and modern memory management.
* **AI-Oriented Numeric Types:** Supports `bfloat16`, `half` (FP16), and quantized integer types (`qint8`, `quint8`, `qint16`, `quint16`) with compatibility across both CPU and CUDA-enabled GPU environments.
* **Lightweight & Dependency-Free:** Header-first, modular architecture with zero external dependencies.
* **CUDA Support:** Write seamless host (CPU) and device (GPU) code using the `TLX_HAS_CUDA` macro and `TLX_HD` annotations.
* **Advanced I/O and File System Utilities:** Includes `vstring`, `File` abstractions, and high-performance binary serialization through `BinaryReader` and `BinaryWriter`.
* **High-Resolution Timing & Profiling:** Provides the `StopWatch` utility with microsecond and nanosecond precision.

---

## Core Components and Example Code

### 1. Memory Views with `Span` (Zero-Copy Access)

`tlx::Span` provides a lightweight, non-owning view over contiguous memory, similar to `std::span`, without any copying overhead.

```cpp
#include <tlx/span.hpp>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> data = {10, 20, 30, 40};

    // Create a zero-overhead view over the vector
    tlx::Span<int> view(data.data(), data.size());

    // Double every element
    for (auto& value : view) {
        value *= 2;
    }

    std::cout << "Updated first element: " << view[0] << std::endl; // Output: 20
}
```

### 2. AI Numeric Types (`bfloat16` & `half`)

16-bit floating-point types designed to reduce memory consumption and improve computational performance in machine learning workloads.

```cpp
#include <tlx/types.hpp>
#include <iostream>

int main() {
    // Construct from 32-bit floating-point values
    tlx::bfloat16 fp_bfloat = 3.14159f;
    tlx::half fp_half = 2.71828f;

    // Basic arithmetic
    tlx::bfloat16 result = fp_bfloat + tlx::bfloat16(1.0f);

    std::cout << "bfloat16 Value: " << float(result) << std::endl;
    std::cout << "half Value: " << float(fp_half) << std::endl;
}
```

### 3. High-Performance Binary File I/O (`BinaryWriter` & `BinaryReader`)

Efficiently writes and reads trivially copyable data structures from binary files.

```cpp
#include <tlx/file_system.hpp>
#include <iostream>

struct ModelWeights {
    float bias;
    int layer_id;
};

int main() {
    const std::string file_path = "model.bin";

    // Write binary data
    {
        tlx::fs::BinaryWriter writer(file_path);
        ModelWeights weights = {0.85f, 42};
        writer.write(weights);
    }

    // Read binary data
    {
        tlx::fs::BinaryReader reader(file_path);
        auto loaded_weights = reader.read<ModelWeights>();

        std::cout << "Bias: " << loaded_weights.bias
                  << ", Layer ID: " << loaded_weights.layer_id << std::endl;
    }
}
```

### 4. High-Precision Timing (`StopWatch`)

Measure execution time with microsecond or nanosecond precision.

```cpp
#include <tlx/time.hpp>
#include <thread>
#include <iostream>

int main() {
    tlx::StopWatch stopwatch;

    // Simulated workload
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    auto elapsed = stopwatch.elapsed<tlx::microseconds>();
    std::cout << "Elapsed time: " << elapsed << " microseconds." << std::endl;
}
```

---

## Building and Testing

TLX uses the modern **CMake** build system.

### Requirements

* A C++20-compatible compiler (GCC 11+, Clang 13+, MSVC 2022+)
* CMake (version 3.20 or newer recommended)
* NVIDIA CUDA Toolkit (optional, for GPU support)

### Build Locally and Run Tests

```bash
# Clone the repository
git clone https://github.com/KayaxcOff/TLX.git
cd TLX

# Configure the build
cmake -B build

# Build the library and tests
cmake --build build

# Run the tests
./build/TLX_HOST_TEST
```

---

## Integrating into Your Project (CMake)

You can integrate TLX into your CMake project using `FetchContent`:

```cmake
include(FetchContent)

FetchContent_Declare(
    TLX
    GIT_REPOSITORY https://github.com/KayaxcOff/TLX.git
    GIT_TAG        main # You can also use a specific commit hash or tag
)

FetchContent_MakeAvailable(TLX)

# Link TLX to your target
target_link_libraries(YourApplication PRIVATE TLX::TLX)
```

---

## Roadmap

- [ ] Advanced SIMD (AVX2 / AVX-512) integration
- [ ] Additional AI-focused data structures and tensor utilities
- [ ] Expanded file system utilities and JSON/YAML serialization support
- [ ] Comprehensive benchmarking suite

---

## License

This project is licensed under the **MIT License**. See the `LICENSE` file for more information.