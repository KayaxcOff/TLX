enable_testing()

add_executable(TLX_HOST_TEST tests/main.cpp)

target_link_libraries(TLX_HOST_TEST PRIVATE
    TLX
)

add_executable(TLX_CUDA_TEST tests/main.cu)

target_link_libraries(TLX_CUDA_TEST PRIVATE
    TLX
)